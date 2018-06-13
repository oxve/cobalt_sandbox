// Copyright 2018 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef COBALT_MEDIA_STREAM_TESTING_MOCK_MEDIA_STREAM_AUDIO_SINK_H_
#define COBALT_MEDIA_STREAM_TESTING_MOCK_MEDIA_STREAM_AUDIO_SINK_H_

#include "cobalt/media_stream/media_stream_audio_sink.h"

#include "testing/gmock/include/gmock/gmock.h"

namespace cobalt {
namespace media_stream {

class MockMediaStreamAudioSink : public MediaStreamAudioSink {
 public:
  MOCK_METHOD2(OnData, void(const ShellAudioBus& audio_bus,
                            base::TimeTicks reference_time));
  MOCK_METHOD1(OnSetFormat, void(const media_stream::AudioParameters& params));
};

}  // namespace media_stream
}  // namespace cobalt

#endif  // COBALT_MEDIA_STREAM_TESTING_MOCK_MEDIA_STREAM_AUDIO_SINK_H_
