#include "audio_output/audio_output.hpp"
#include "pipewire/stream.h"
#include "pipewire/thread-loop.h"
#include <iostream>
#include <math.h>
#include <pipewire/pipewire.h>
#include <spa/param/audio/format-utils.h>

using namespace soundwich;

PipeWireCore::PipeWireCore()
{
    std::ranges::fill(items, nullptr);

    rate = 44100;
    channels = 2;
    pw_init(NULL, NULL);

    const struct spa_pod *params[1];
    struct spa_pod_builder b = SPA_POD_BUILDER_INIT((void *)pod_buffer.data(), sizeof(pod_buffer));


    loop = pw_thread_loop_new("loop", NULL);

    stream_events.version = PW_VERSION_STREAM_EVENTS;
    stream_events.process = process;

    stream = pw_stream_new_simple(
        pw_thread_loop_get_loop(loop), "audio-src",
        pw_properties_new(PW_KEY_MEDIA_TYPE, "Audio", PW_KEY_MEDIA_CATEGORY,
                          "Playback", PW_KEY_MEDIA_ROLE, "Music", NULL),
        &stream_events, this);

    spa_audio_info_raw tmp = SPA_AUDIO_INFO_RAW_INIT(.format = SPA_AUDIO_FORMAT_F32,
                                                     .rate = rate,
                                                     .channels = channels);

    params[0] = spa_format_audio_raw_build(&b, SPA_PARAM_EnumFormat, &tmp);

    pw_stream_connect(stream, PW_DIRECTION_OUTPUT, PW_ID_ANY,
                      (pw_stream_flags)(PW_STREAM_FLAG_AUTOCONNECT | PW_STREAM_FLAG_MAP_BUFFERS |
                                        PW_STREAM_FLAG_RT_PROCESS),
                      params, 1);

    pw_thread_loop_start(loop);
}

PipeWireOutput &PipeWireCore::createPipeWireOutput()
{
    int ind = -1;
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i] == nullptr)
        {
            ind = i;
            break;
        }
    }
    if (ind == -1)
        throw "aboba";

    PipeWireOutput* res = new PipeWireOutput();
    res->core = this;
    res->ind = ind;

    items[ind] = res;
    return *items[ind];
}

PipeWireCore::~PipeWireCore()
{
    pw_thread_loop_stop(loop);
    pw_stream_destroy(stream);
}

void PipeWireCore::changeSettings(uint32_t newRate, uint32_t newChannels) {
    pw_thread_loop_lock(loop);

    bool was_active = false;
    if (pw_stream_get_state(stream, nullptr) == PW_STREAM_STATE_STREAMING) {
        pw_stream_set_active(stream, false);
        was_active = true;
    }

    rate = newRate;
    channels = newChannels;
    spa_audio_info_raw tmp = SPA_AUDIO_INFO_RAW_INIT(.format = SPA_AUDIO_FORMAT_F32,
                                                     .rate = rate,
                                                     .channels = channels);
    const struct spa_pod *params[1];
    struct spa_pod_builder b = SPA_POD_BUILDER_INIT((void *)pod_buffer.data(), sizeof(pod_buffer));
    
    params[0] = spa_format_audio_raw_build(&b, SPA_PARAM_EnumFormat, &tmp);

    pw_stream_update_params(stream, params, 1);

    if (was_active) {
        pw_stream_set_active(stream, true);
    }

    pw_thread_loop_unlock(loop);
}
