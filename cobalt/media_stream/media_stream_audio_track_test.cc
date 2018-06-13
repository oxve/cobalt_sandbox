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

#include "cobalt/media_stream/media_stream_audio_track.h"

#include "cobalt/media_stream/media_stream_audio_deliverer.h"
#include "cobalt/media_stream/testing/mock_media_stream_audio_sink.h"
#include "testing/gtest/include/gtest/gtest.h"

#if defined(COBALT_MEDIA_SOURCE_2016)
#include "cobalt/media/base/shell_audio_bus.h"
#else  // defined(COBALT_MEDIA_SOURCE_2016)
#include "media/base/shell_audio_bus.h"
#endif  // defined(COBALT_MEDIA_SOURCE_2016)

using ::testing::_;
using ::testing::StrictMock;

namespace {

const int kFrameCount = 20;
const int kChannelCount = 1;
const int kSampleRate = 1000;
const int kBitsPerSample = 8;

}  // namespace

namespace cobalt {
namespace media_stream {

#if defined(COBALT_MEDIA_SOURCE_2016)
typedef media::ShellAudioBus ShellAudioBus;
#else   // defined(COBALT_MEDIA_SOURCE_2016)
typedef ::media::ShellAudioBus ShellAudioBus;
#endif  // defined(COBALT_MEDIA_SOURCE_2016)

TEST(MediaStreamAudioTrack, OnSetFormatAndData) {
  media_stream::AudioParameters expected_params(kChannelCount, kSampleRate,
                                                kBitsPerSample);
  base::TimeTicks expected_time = base::TimeTicks::Now();

  StrictMock<MockMediaStreamAudioSink> mock_sink;
  EXPECT_CALL(mock_sink, OnSetFormat(expected_params));
  EXPECT_CALL(mock_sink, OnData(_, expected_time));

  scoped_refptr<MediaStreamAudioTrack> track = new MediaStreamAudioTrack();
  track->AddSink(&mock_sink);

  MediaStreamAudioDeliverer<MediaStreamAudioTrack> deliverer;
  deliverer.AddConsumer(track);

  deliverer.OnSetFormat(expected_params);

  ShellAudioBus audio_bus(kChannelCount, kFrameCount, ShellAudioBus::kInt16,
                          ShellAudioBus::kInterleaved);

  deliverer.OnData(audio_bus, expected_time);
}

TEST(MediaStreamAudioTrack, OneTrackMultipleSinks) {
  media_stream::AudioParameters expected_params(kChannelCount, kSampleRate,
                                                kBitsPerSample);
  base::TimeTicks expected_time = base::TimeTicks::Now();

  StrictMock<MockMediaStreamAudioSink> mock_sink1;
  EXPECT_CALL(mock_sink1, OnSetFormat(expected_params));
  EXPECT_CALL(mock_sink1, OnData(_, expected_time));
  StrictMock<MockMediaStreamAudioSink> mock_sink2;
  EXPECT_CALL(mock_sink2, OnSetFormat(expected_params));
  EXPECT_CALL(mock_sink2, OnData(_, expected_time));

  scoped_refptr<MediaStreamAudioTrack> track = new MediaStreamAudioTrack();
  track->AddSink(&mock_sink1);
  track->AddSink(&mock_sink2);

  MediaStreamAudioDeliverer<MediaStreamAudioTrack> deliverer;
  deliverer.AddConsumer(track);

  deliverer.OnSetFormat(expected_params);

  ShellAudioBus audio_bus(kChannelCount, kFrameCount, ShellAudioBus::kInt16,
                          ShellAudioBus::kInterleaved);

  deliverer.OnData(audio_bus, expected_time);
}

TEST(MediaStreamAudioTrack, TwoTracksWithOneSinkEach) {
  media_stream::AudioParameters expected_params(kChannelCount, kSampleRate,
                                                kBitsPerSample);
  base::TimeTicks expected_time = base::TimeTicks::Now();

  StrictMock<MockMediaStreamAudioSink> mock_sink1;
  EXPECT_CALL(mock_sink1, OnSetFormat(expected_params));
  EXPECT_CALL(mock_sink1, OnData(_, expected_time));
  StrictMock<MockMediaStreamAudioSink> mock_sink2;
  EXPECT_CALL(mock_sink2, OnSetFormat(expected_params));
  EXPECT_CALL(mock_sink2, OnData(_, expected_time));

  scoped_refptr<MediaStreamAudioTrack> track1 = new MediaStreamAudioTrack();
  scoped_refptr<MediaStreamAudioTrack> track2 = new MediaStreamAudioTrack();
  track1->AddSink(&mock_sink1);
  track2->AddSink(&mock_sink2);

  MediaStreamAudioDeliverer<MediaStreamAudioTrack> deliverer;
  deliverer.AddConsumer(track1);
  deliverer.AddConsumer(track2);

  deliverer.OnSetFormat(expected_params);

  ShellAudioBus audio_bus(kChannelCount, kFrameCount, ShellAudioBus::kInt16,
                          ShellAudioBus::kInterleaved);

  deliverer.OnData(audio_bus, expected_time);
}

TEST(MediaStreamAudioTrack, AddRemoveSink) {
  media_stream::AudioParameters expected_params(kChannelCount, kSampleRate,
                                                kBitsPerSample);
  base::TimeTicks expected_time = base::TimeTicks::Now();

  StrictMock<MockMediaStreamAudioSink> mock_sink;

  scoped_refptr<MediaStreamAudioTrack> track = new MediaStreamAudioTrack();
  track->AddSink(&mock_sink);

  MediaStreamAudioDeliverer<MediaStreamAudioTrack> deliverer;
  deliverer.AddConsumer(track);

  deliverer.OnSetFormat(expected_params);

  ShellAudioBus audio_bus(kChannelCount, kFrameCount, ShellAudioBus::kInt16,
                          ShellAudioBus::kInterleaved);

  track->RemoveSink(&mock_sink);
  deliverer.OnData(audio_bus, expected_time);
}

}  // namespace media_stream
}  // namespace cobalt
