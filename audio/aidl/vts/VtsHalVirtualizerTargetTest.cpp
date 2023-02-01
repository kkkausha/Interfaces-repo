/*
 * Copyright (C) 2022 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "VtsHalVirtualizerTest"

#include <Utils.h>
#include <aidl/Vintf.h>
#include "EffectHelper.h"

using namespace android;

using aidl::android::hardware::audio::effect::Capability;
using aidl::android::hardware::audio::effect::Descriptor;
using aidl::android::hardware::audio::effect::IEffect;
using aidl::android::hardware::audio::effect::IFactory;
using aidl::android::hardware::audio::effect::kVirtualizerTypeUUID;
using aidl::android::hardware::audio::effect::Parameter;
using aidl::android::hardware::audio::effect::Virtualizer;

/**
 * Here we focus on specific parameter checking, general IEffect interfaces testing performed in
 * VtsAudioEffectTargetTest.
 */
enum ParamName { PARAM_INSTANCE_NAME, PARAM_STRENGTH };
using VirtualizerParamTestParam = std::tuple<std::pair<std::shared_ptr<IFactory>, Descriptor>, int>;

/*
 * Testing parameter range, assuming the parameter supported by effect is in this range.
 * Parameter should be within the valid range defined in the documentation,
 * for any supported value test expects EX_NONE from IEffect.setParameter(),
 * otherwise expect EX_ILLEGAL_ARGUMENT.
 */

class VirtualizerParamTest : public ::testing::TestWithParam<VirtualizerParamTestParam>,
                             public EffectHelper {
  public:
    VirtualizerParamTest() : mParamStrength(std::get<PARAM_STRENGTH>(GetParam())) {
        std::tie(mFactory, mDescriptor) = std::get<PARAM_INSTANCE_NAME>(GetParam());
    }

    void SetUp() override {
        ASSERT_NE(nullptr, mFactory);
        ASSERT_NO_FATAL_FAILURE(create(mFactory, mEffect, mDescriptor));

        Parameter::Specific specific = getDefaultParamSpecific();
        Parameter::Common common = EffectHelper::createParamCommon(
                0 /* session */, 1 /* ioHandle */, 44100 /* iSampleRate */, 44100 /* oSampleRate */,
                kInputFrameCount /* iFrameCount */, kOutputFrameCount /* oFrameCount */);
        IEffect::OpenEffectReturn ret;
        ASSERT_NO_FATAL_FAILURE(open(mEffect, common, specific, &ret, EX_NONE));
        ASSERT_NE(nullptr, mEffect);
    }

    void TearDown() override {
        ASSERT_NO_FATAL_FAILURE(close(mEffect));
        ASSERT_NO_FATAL_FAILURE(destroy(mFactory, mEffect));
    }

    Parameter::Specific getDefaultParamSpecific() {
        Virtualizer vr = Virtualizer::make<Virtualizer::strengthPm>(0);
        Parameter::Specific specific =
                Parameter::Specific::make<Parameter::Specific::virtualizer>(vr);
        return specific;
    }

    static const long kInputFrameCount = 0x100, kOutputFrameCount = 0x100;
    std::shared_ptr<IFactory> mFactory;
    std::shared_ptr<IEffect> mEffect;
    Descriptor mDescriptor;
    int mParamStrength = 0;

    void SetAndGetVirtualizerParameters() {
        for (auto& it : mTags) {
            auto& tag = it.first;
            auto& vr = it.second;

            // validate parameter
            Descriptor desc;
            ASSERT_STATUS(EX_NONE, mEffect->getDescriptor(&desc));
            const bool valid = isTagInRange(it.first, it.second, desc);
            const binder_exception_t expected = valid ? EX_NONE : EX_ILLEGAL_ARGUMENT;

            // set parameter
            Parameter expectParam;
            Parameter::Specific specific;
            specific.set<Parameter::Specific::virtualizer>(vr);
            expectParam.set<Parameter::specific>(specific);
            EXPECT_STATUS(expected, mEffect->setParameter(expectParam)) << expectParam.toString();

            // only get if parameter in range and set success
            if (expected == EX_NONE) {
                Parameter getParam;
                Parameter::Id id;
                Virtualizer::Id vrId;
                vrId.set<Virtualizer::Id::commonTag>(tag);
                id.set<Parameter::Id::virtualizerTag>(vrId);
                // if set success, then get should match
                EXPECT_STATUS(expected, mEffect->getParameter(id, &getParam));
                EXPECT_EQ(expectParam, getParam);
            }
        }
    }

    void addStrengthParam(int strength) {
        Virtualizer vr;
        vr.set<Virtualizer::strengthPm>(strength);
        mTags.push_back({Virtualizer::strengthPm, vr});
    }

    bool isTagInRange(const Virtualizer::Tag& tag, const Virtualizer& vr,
                      const Descriptor& desc) const {
        const Virtualizer::Capability& vrCap = desc.capability.get<Capability::virtualizer>();
        switch (tag) {
            case Virtualizer::strengthPm: {
                int strength = vr.get<Virtualizer::strengthPm>();
                return isStrengthInRange(vrCap, strength);
            }
            default:
                return false;
        }
        return false;
    }

    bool isStrengthInRange(const Virtualizer::Capability& cap, int strength) const {
        return cap.strengthSupported && strength >= 0 && strength <= cap.maxStrengthPm;
    }

    static std::vector<int> getStrengthTestValues(
            std::vector<std::pair<std::shared_ptr<IFactory>, Descriptor>> kFactoryDescList) {
        const auto max = std::max_element(
                kFactoryDescList.begin(), kFactoryDescList.end(),
                [](const std::pair<std::shared_ptr<IFactory>, Descriptor>& a,
                   const std::pair<std::shared_ptr<IFactory>, Descriptor>& b) {
                    return a.second.capability.get<Capability::virtualizer>().maxStrengthPm <
                           b.second.capability.get<Capability::virtualizer>().maxStrengthPm;
                });
        if (max == kFactoryDescList.end()) {
            return {0};
        }
        int maxStrength = max->second.capability.get<Capability::virtualizer>().maxStrengthPm;
        return {std::numeric_limits<int>::min(),
                -1,
                0,
                maxStrength >> 1,
                maxStrength,
                maxStrength + 1,
                std::numeric_limits<int>::max()};
    }

  private:
    std::vector<std::pair<Virtualizer::Tag, Virtualizer>> mTags;
    void CleanUp() { mTags.clear(); }
};

TEST_P(VirtualizerParamTest, SetAndGetStrength) {
    EXPECT_NO_FATAL_FAILURE(addStrengthParam(mParamStrength));
    SetAndGetVirtualizerParameters();
}

INSTANTIATE_TEST_SUITE_P(
        VirtualizerTest, VirtualizerParamTest,
        ::testing::Combine(testing::ValuesIn(EffectFactoryHelper::getAllEffectDescriptors(
                                   IFactory::descriptor, kVirtualizerTypeUUID)),
                           testing::ValuesIn(VirtualizerParamTest::getStrengthTestValues(
                                   EffectFactoryHelper::getAllEffectDescriptors(
                                           IFactory::descriptor, kVirtualizerTypeUUID)))),
        [](const testing::TestParamInfo<VirtualizerParamTest::ParamType>& info) {
            auto descriptor = std::get<PARAM_INSTANCE_NAME>(info.param).second;
            std::string strength = std::to_string(std::get<PARAM_STRENGTH>(info.param));
            std::string name = "Implementor_" + descriptor.common.implementor + "_name_" +
                               descriptor.common.name + "_UUID_" +
                               descriptor.common.id.uuid.toString() + "_strength" + strength;
            std::replace_if(
                    name.begin(), name.end(), [](const char c) { return !std::isalnum(c); }, '_');
            return name;
        });

GTEST_ALLOW_UNINSTANTIATED_PARAMETERIZED_TEST(VirtualizerParamTest);

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ABinderProcess_setThreadPoolMaxThreadCount(1);
    ABinderProcess_startThreadPool();
    return RUN_ALL_TESTS();
}
