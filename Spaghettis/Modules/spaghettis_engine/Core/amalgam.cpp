
/* Copyright (c) 1997 Miller Puckette and others. */

/* < https://opensource.org/licenses/BSD-3-Clause > */

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#include "m_spaghettis.h"

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

namespace spaghettis::core {

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

/* Core. */

#include "core/global/m_global.c"
#include "core/global/m_setup.c"
#include "core/global/m_legacy.c"

#include "core/instance/m_pool.c"
#include "core/instance/m_register.c"
#include "core/instance/m_clocks_safe.c"
#include "core/instance/m_clocks_single.c"
#include "core/instance/m_clocks.c"
#include "core/instance/m_abstractions.c"
#include "core/instance/m_eval.c"
#include "core/instance/m_stack.c"
#include "core/instance/m_load.c"
#include "core/instance/m_view.c"
#include "core/instance/m_environment.c"
#include "core/instance/m_autorelease.c"
#include "core/instance/m_polling.c"
#include "core/instance/m_pending.c"
#include "core/instance/m_snap.c"
#include "core/instance/m_instance.c"

#include "core/message/m_symbols.c"
#include "core/message/m_bindlist.c"
#include "core/message/m_bind.c"
#include "core/message/m_symbol.c"
#include "core/message/m_message.c"
#include "core/message/m_method.c"

#include "core/class/m_class.c"
#include "core/class/m_pd.c"
#include "core/class/m_object.c"
#include "core/class/m_identifiers.c"
#include "core/class/m_inlet.c"
#include "core/class/m_outlet.c"

#include "core/buffer/m_atom.c"
#include "core/buffer/m_buffer.c"
#include "core/buffer/m_append.c"
#include "core/buffer/m_extend.c"
#include "core/buffer/m_slot.c"
#include "core/buffer/m_parse.c"
#include "core/buffer/m_file.c"

#include "core/clipboard/m_clipboard.c"
#include "core/clipboard/m_encapsulate.c"
#include "core/clipboard/m_deencapsulate.c"
#include "core/clipboard/m_snippet.c"

#include "core/helpers/m_rectangle.c"
#include "core/helpers/m_fileproperties.c"
#include "core/helpers/m_iterator.c"
#include "core/helpers/m_pathlist.c"
#include "core/helpers/m_heapstring.c"
#include "core/helpers/m_items.c"
#include "core/helpers/m_MT.c"
#include "core/helpers/m_MT_32.c"
#include "core/helpers/m_MT_64.c"
#include "core/helpers/m_MT_i.c"

#include "core/utils/m_utils.c"
#include "core/utils/m_symbol.c"
#include "core/utils/m_math.c"
#include "core/utils/m_string.c"
#include "core/utils/m_path.c"
#include "core/utils/m_error.c"

/* Wrapper. */

#include "wrap/w_wrapper.c"
#include "wrap/w_inputs.c"
#include "wrap/w_outputs.c"
#include "wrap/w_unique.c"

/* Undo. */

#include "undo/m_undosnippet.c"
#include "undo/m_undoaction.c"
#include "undo/m_undomanager.c"
#include "undo/m_undosafe.c"
#include "undo/m_undotrigger.c"
#include "undo/m_undocollapse.c"

#include "undo/actions/m_undoseparator.c"
#include "undo/actions/m_undoadd.c"
#include "undo/actions/m_undoremove.c"
#include "undo/actions/m_undocut.c"
#include "undo/actions/m_undopaste.c"
#include "undo/actions/m_undoduplicate.c"
#include "undo/actions/m_undosnap.c"
#include "undo/actions/m_undoencapsulate.c"
#include "undo/actions/m_undodeencapsulate.c"
#include "undo/actions/m_undoconnect.c"
#include "undo/actions/m_undodisconnect.c"
#include "undo/actions/m_undomotion.c"
#include "undo/actions/m_undoresize.c"
#include "undo/actions/m_undocreate.c"
#include "undo/actions/m_undodelete.c"
#include "undo/actions/m_undofront.c"
#include "undo/actions/m_undoback.c"

/* System. */

#include "system/memory/s_memory.c"
#include "system/memory/s_leak.c"
#include "system/memory/s_atomic.c"
#include "system/memory/s_trylock.c"
#include "system/memory/s_fifo.c"
#include "system/memory/s_fifo8.c"
#include "system/memory/s_fifo32.c"
#include "system/memory/s_ring.c"

#include "system/scheduler/s_monitor.c"
#include "system/scheduler/s_clock.c"
#include "system/scheduler/s_time.c"
#include "system/scheduler/s_scheduler.c"
#include "system/scheduler/s_startup.c"
#include "system/scheduler/s_main.c"
#include "system/scheduler/s_entry.c"

#include "system/settings/s_properties.c"
#include "system/settings/s_settings.c"

#include "system/files/s_file.c"
#include "system/files/s_searchpath.c"

#include "system/log/s_debug.c"
#include "system/log/s_post.c"
#include "system/log/s_logger.c"

#include "system/policy/s_denormal.c"
#include "system/policy/s_priority.c"
#include "system/policy/s_privilege.c"
#include "system/policy/s_handlers.c"

#include "system/audio/s_audio.c"

#include "system/MIDI/s_midi.c"
#include "system/MIDI/s_inmidi.c"
#include "system/MIDI/s_outmidi.c"

#include "system/devices/s_devices.c"
#include "system/devices/s_deviceslist.c"
#include "system/devices/s_devicesfetch.c"

#include "system/API/s_loader.c"
#include "system/API/s_metadata.c"
#include "system/API/s_midi_API.c"
#include "system/API/s_audio_API.c"

/* Graphic. */

#include "graphic/patch/g_base.c"
#include "graphic/patch/g_glist.c"
#include "graphic/patch/g_others.c"
#include "graphic/patch/g_undo.c"
#include "graphic/patch/g_add.c"
#include "graphic/patch/g_select.c"
#include "graphic/patch/g_index.c"
#include "graphic/patch/g_arrange.c"
#include "graphic/patch/g_line.c"
#include "graphic/patch/g_outlet.c"
#include "graphic/patch/g_paste.c"
#include "graphic/patch/g_file.c"
#include "graphic/patch/g_traverser.c"
#include "graphic/patch/g_dollar.c"

#include "graphic/objects/g_gui.c"
#include "graphic/objects/g_canvas.c"
#include "graphic/objects/g_vinlet.c"
#include "graphic/objects/g_voutlet.c"
#include "graphic/objects/g_comment.c"
#include "graphic/objects/g_message.c"
#include "graphic/objects/g_gatom.c"
#include "graphic/objects/g_garray.c"
#include "graphic/objects/g_bang.c"
#include "graphic/objects/g_toggle.c"
#include "graphic/objects/g_radio.c"
#include "graphic/objects/g_slider.c"
#include "graphic/objects/g_dial.c"
#include "graphic/objects/g_vu.c"

/* Control. */

#include "control/x_atomoutlet.c"
#include "control/x_tinyexpr.c"

#include "control/atom/x_int.c"
#include "control/atom/x_float.c"
#include "control/atom/x_symbol.c"
#include "control/atom/x_bang.c"

#include "control/list/x_list.c"
#include "control/list/x_listinlet.c"
#include "control/list/x_listappend.c"
#include "control/list/x_listchange.c"
#include "control/list/x_listequal.c"
#include "control/list/x_listfromsymbol.c"
#include "control/list/x_listgroup.c"
#include "control/list/x_listiterate.c"
#include "control/list/x_listlength.c"
#include "control/list/x_listprepend.c"
#include "control/list/x_listreverse.c"
#include "control/list/x_listrotate.c"
#include "control/list/x_listscramble.c"
#include "control/list/x_listsort.c"
#include "control/list/x_listsplit.c"
#include "control/list/x_liststore.c"
#include "control/list/x_liststream.c"
#include "control/list/x_listsum.c"
#include "control/list/x_listtosymbol.c"
#include "control/list/x_listtrim.c"

#include "control/text/x_textbuffer.c"
#include "control/text/x_textclient.c"
#include "control/text/x_text.c"
#include "control/text/x_textget.c"
#include "control/text/x_textset.c"
#include "control/text/x_textinsert.c"
#include "control/text/x_textdelete.c"
#include "control/text/x_textsize.c"
#include "control/text/x_textlist.c"
#include "control/text/x_textsearch.c"
#include "control/text/x_textsequence.c"
#include "control/text/x_qlist.c"
#include "control/text/x_textfile.c"

#include "control/array/x_array.c"
#include "control/array/x_arrayclient.c"
#include "control/array/x_arrayrange.c"
#include "control/array/x_arraysize.c"
#include "control/array/x_arraysum.c"
#include "control/array/x_arrayget.c"
#include "control/array/x_arrayset.c"
#include "control/array/x_arrayquantile.c"
#include "control/array/x_arrayrandom.c"
#include "control/array/x_arraymax.c"
#include "control/array/x_arraymin.c"

#include "control/tab/x_tabwrite.c"
#include "control/tab/x_tabread.c"
#include "control/tab/x_tabread4.c"

#include "control/math/x_acoustic.c"
#include "control/math/x_math.c"
#include "control/math/x_binop1.c"
#include "control/math/x_binop2.c"
#include "control/math/x_binop3.c"
#include "control/math/x_atan2.c"
#include "control/math/x_random.c"
#include "control/math/x_urn.c"
#include "control/math/x_clip.c"
#include "control/math/x_expr.c"
#include "control/math/x_vexpr.c"

#include "control/time/x_metro.c"
#include "control/time/x_delay.c"
#include "control/time/x_line.c"
#include "control/time/x_pipe.c"

#include "control/flow/x_send.c"
#include "control/flow/x_receive.c"
#include "control/flow/x_select.c"
#include "control/flow/x_route.c"
#include "control/flow/x_pack.c"
#include "control/flow/x_unpack.c"
#include "control/flow/x_trigger.c"
#include "control/flow/x_prepend.c"
#include "control/flow/x_spigot.c"
#include "control/flow/x_moses.c"
#include "control/flow/x_until.c"
#include "control/flow/x_uzi.c"
#include "control/flow/x_swap.c"
#include "control/flow/x_change.c"
#include "control/flow/x_value.c"
#include "control/flow/x_counter.c"

#include "control/miscellaneous/x_print.c"
#include "control/miscellaneous/x_makefilename.c"
#include "control/miscellaneous/x_loadbang.c"
#include "control/miscellaneous/x_loadmess.c"
#include "control/miscellaneous/x_closebang.c"
#include "control/miscellaneous/x_samplerate.c"
#include "control/miscellaneous/x_blocksize.c"
#include "control/miscellaneous/x_dspstatus.c"
#include "control/miscellaneous/x_arguments.c"
#include "control/miscellaneous/x_directories.c"
#include "control/miscellaneous/x_freeze.c"
#include "control/miscellaneous/x_timer.c"
#include "control/miscellaneous/x_realtime.c"

#include "control/network/x_receiver.c"
#include "control/network/x_netsend.c"
#include "control/network/x_netreceive.c"
#include "control/network/x_oscparse.c"
#include "control/network/x_oscformat.c"
#include "control/network/x_oscbundle.c"
#include "control/network/x_oscstream.c"
#include "control/network/x_timestamp.c"

#include "control/midi/x_midiports.c"
#include "control/midi/x_makenote.c"
#include "control/midi/x_stripnote.c"
#include "control/midi/x_bag.c"
#include "control/midi/x_poly.c"
#include "control/midi/x_notein.c"
#include "control/midi/x_noteout.c"
#include "control/midi/x_ctlin.c"
#include "control/midi/x_ctlout.c"
#include "control/midi/x_pgmin.c"
#include "control/midi/x_pgmout.c"
#include "control/midi/x_bendin.c"
#include "control/midi/x_bendout.c"
#include "control/midi/x_touchin.c"
#include "control/midi/x_touchout.c"
#include "control/midi/x_polytouchin.c"
#include "control/midi/x_polytouchout.c"
#include "control/midi/x_sysexin.c"
#include "control/midi/x_sysexout.c"
#include "control/midi/x_midisystemin.c"
#include "control/midi/x_midisystemout.c"

/* DSP. */

#include "dsp/graph/d_dsp.c"
#include "dsp/graph/d_dspthread.c"
#include "dsp/graph/d_chain.c"
#include "dsp/graph/d_closures.c"
#include "dsp/graph/d_initializer.c"
#include "dsp/graph/d_signal.c"
#include "dsp/graph/d_ugen.c"
#include "dsp/graph/d_canvas.c"
#include "dsp/graph/d_vinlet.c"
#include "dsp/graph/d_voutlet.c"
#include "dsp/graph/d_adc.c"
#include "dsp/graph/d_dac.c"
#include "dsp/graph/d_resample.c"
#include "dsp/graph/d_block.c"
#include "dsp/graph/d_vperform.c"
#include "dsp/graph/d_perform.c"
#include "dsp/graph/d_functions.c"

#include "dsp/global/d_throw.c"
#include "dsp/global/d_catch.c"
#include "dsp/global/d_send.c"
#include "dsp/global/d_receive.c"

#include "dsp/osc/d_osc.c"
#include "dsp/osc/d_phasor.c"
#include "dsp/osc/d_cos.c"

#include "dsp/convert/d_sig.c"
#include "dsp/convert/d_line.c"
#include "dsp/convert/d_snapshot.c"
#include "dsp/convert/d_env.c"
#include "dsp/convert/d_threshold.c"
#include "dsp/convert/d_samphold.c"

#include "dsp/delay/d_delwrite.c"
#include "dsp/delay/d_delread.c"
#include "dsp/delay/d_vd.c"

#include "dsp/tab/d_tabwrite.c"
#include "dsp/tab/d_tabread.c"
#include "dsp/tab/d_tabread4.c"
#include "dsp/tab/d_tabplay.c"
#include "dsp/tab/d_tabosc4.c"
#include "dsp/tab/d_tabsend.c"
#include "dsp/tab/d_tabreceive.c"

#include "dsp/math/d_abs.c"
#include "dsp/math/d_wrap.c"
#include "dsp/math/d_sqrt.c"
#include "dsp/math/d_rsqrt.c"
#include "dsp/math/d_exp.c"
#include "dsp/math/d_mtof.c"
#include "dsp/math/d_ftom.c"
#include "dsp/math/d_dbtorms.c"
#include "dsp/math/d_rmstodb.c"
#include "dsp/math/d_dbtopow.c"
#include "dsp/math/d_powtodb.c"
#include "dsp/math/d_add.c"
#include "dsp/math/d_subtract.c"
#include "dsp/math/d_multiply.c"
#include "dsp/math/d_divide.c"
#include "dsp/math/d_greater.c"
#include "dsp/math/d_less.c"
#include "dsp/math/d_max.c"
#include "dsp/math/d_min.c"
#include "dsp/math/d_clip.c"
#include "dsp/math/d_log.c"
#include "dsp/math/d_pow.c"

#include "dsp/miscellaneous/d_print.c"
#include "dsp/miscellaneous/d_bang.c"
#include "dsp/miscellaneous/d_noise.c"
#include "dsp/miscellaneous/d_lrshift.c"

#include "dsp/soundfile/d_soundfile.c"
#include "dsp/soundfile/d_subchunk.c"
#include "dsp/soundfile/d_codec.c"
#include "dsp/soundfile/d_soundfiler.c"
#include "dsp/soundfile/d_soundinfo.c"
#include "dsp/soundfile/d_sfthread.c"
#include "dsp/soundfile/d_readsf.c"
#include "dsp/soundfile/d_writesf.c"

#include "dsp/filters/d_vcf.c"
#include "dsp/filters/d_hip.c"
#include "dsp/filters/d_lop.c"
#include "dsp/filters/d_bp.c"
#include "dsp/filters/d_biquad.c"
#include "dsp/filters/d_rpole.c"
#include "dsp/filters/d_rzero.c"
#include "dsp/filters/d_rzeroreverse.c"
#include "dsp/filters/d_cpole.c"
#include "dsp/filters/d_czero.c"
#include "dsp/filters/d_czeroreverse.c"

#include "dsp/fft/d_rfft.c"
#include "dsp/fft/d_rifft.c"
#include "dsp/fft/d_fft.c"
#include "dsp/fft/d_ifft.c"
#include "dsp/fft/d_framp.c"
#include "dsp/fft/d_mag.c"
#include "dsp/fft/d_rmag.c"
#include "dsp/fft/d_sigmund.c"

/* FFT library. */

#include "dsp/d_fftOOURA.c"
#include "dsp/fftsg.c"

/* Audio / MIDI. */

#if defined ( PD_BUILDING_TESTS )

    #include "system/API/s_midi_dummy.c"
    #include "system/API/s_audio_dummy.c"

#else

#if PD_APPLE

    #include "system/API/s_midi_mac.c"
    #include "system/API/s_audio_mac.c"

    #include "libs/core/core_audiodevice.c"
    #include "libs/core/core_audiodevicelist.c"
    #include "libs/core/core_audiocallbacks.c"
    #include "libs/core/core_audiostream.c"
    #include "libs/core/core_audiograph.c"
    
    #include "libs/core/core_midiname.c"
    #include "libs/core/core_mididevicelist.c"
    #include "libs/core/core_midiclient.c"
    #include "libs/core/core_midiport.c"

#endif

#if PD_LINUX
    #include "system/API/s_midi_linux.c"
    #include "system/API/s_audio_linux.c"
#endif

/* Exported for externals. */

#include "libs/externals/spaghettis_externals.c"
#include "libs/externals/spaghettis_space.c"

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------

#if defined ( PD_BUILDING_APPLICATION )

}

#endif

// -----------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------
