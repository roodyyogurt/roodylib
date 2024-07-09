! ---------------------------------------------------------------------------
!
!  Hugor opcode routines.
!
!  This file is in the Public Domain. It can be used, modified and
!  distributed without any restrictions and for any purpose.
!
!  Routines for executing opcodes in the Hugor interpreter. These routines
!  are specific to Hugor and are not supported in other interpreters.
!
!  It is recommended to detect whether the game is running in Hugor first,
!  by using the HugorCheck() routine, before using any of the other routines.
!  You should do that at game start and after restoring saved games. Failure
!  to do so will result in ill effects when the game is not running in Hugor,
!  or when the game was saved in Hugor but restored in another interpreter.
!
! ---------------------------------------------------------------------------
#ifclear _HUGOROPC_H
#set _HUGOROPC_H

! ---------------------------------------------------------------------------
! Opcode array. Opcodes that have return values write them into this array.
! If an opcode routine returns something else than HGR_RESULT_OK, or if the
! opcode does not return output values, nothing is written to the array.
! ---------------------------------------------------------------------------
constant HGR_MAX_OP_ARGS 7
array hugor_op_array[HGR_MAX_OP_ARGS]


! ---------------------------------------------------------------------------
! Opcode return status codes. The opcode routines return one of these status
! codes. HGR_RESULT_OK indicates that the opcode executed without errors. Any
! other status code indicates an error.
! ---------------------------------------------------------------------------
constant HGR_RESULT_OK                   0 ! Success.
constant HGR_RESULT_WRONG_PARAM_COUNT   10 ! Wrong amount of input
                                           ! parameters.
constant HGR_RESULT_WRONG_BYTE_COUNT    20 ! Hugor got an odd amount of bytes
                                           ! (if this ever happens, it's a
                                           ! Hugor bug, not the game's
                                           ! fault.)

! ---------------------------------------------------------------------------
! Operating system codes.
! ---------------------------------------------------------------------------
constant HGR_OS_UNKNOWN  0
constant HGR_OS_WINDOWS  1
constant HGR_OS_MACOSX   2
constant HGR_OS_LINUX    3

! ---------------------------------------------------------------------------
! Returns true if the interpreter is Hugor
! ---------------------------------------------------------------------------
routine HugorCheck()
{
    readfile HGR_CHECK_FILE {
        return readval = HGR_CHECK_MAGIC
    }
}

! ---------------------------------------------------------------------------
! Gets the HGR_OS_* code that corresponds to the operating system the game is
! currently running in, and writes it into the opcode array.
! ---------------------------------------------------------------------------
routine HugorGetOS()
{
    return HugorOpcode(HGR_OP_GET_OS, 0, 1)
}

! ---------------------------------------------------------------------------
! Writes Hugor's major, minor and patch version numbers into the opcode
! array.
! ---------------------------------------------------------------------------
routine HugorGetVersion()
{
    return HugorOpcode(HGR_OP_GET_VERSION, 0, 3)
}

! ---------------------------------------------------------------------------
! Opens the given URL. The URL can be a local file, an Internet URL, an email
! address, or anything else that the OS can automatically open.
! ---------------------------------------------------------------------------
routine HugorOpenUrl(url)
{
    return HugorOpcode(HGR_OP_OPEN_URL, 1, 0, url)
}

! ---------------------------------------------------------------------------
! Fades the screen.
! duration_ms: Fade duration is milliseconds.
! start_alpha: Starting alpha value. If negative, current alpha is used.
! end_alpha: Alpha value to fade to.
! block: If true, execution will halt during the fade. If false, the fade
!        will be performed asynchronously.
! ---------------------------------------------------------------------------
routine HugorFadeScreen(duration_ms, start_alpha, end_alpha, block)
{
    if (start_alpha < 0) {
        start_alpha = -9999
    }
    return HugorOpcode(HGR_OP_FADE_SCREEN, 4, 0, duration_ms, start_alpha,
                       end_alpha, block)
}

! ---------------------------------------------------------------------------
! Enables/disables fullscreen mode.
! f: If true, switch to fullscreen, if false, switch to windowed mode.
! ---------------------------------------------------------------------------
routine HugorSetFullscreen(f)
{
    return HugorOpcode(HGR_OP_SET_FULLSCREEN, 1, 0, f)
}

! ---------------------------------------------------------------------------
! Sets the clipboard to the given string.
! ---------------------------------------------------------------------------
routine HugorSetClipboard(str)
{
    return HugorOpcode(HGR_OP_SET_CLIPBOARD, 1, 0, str)
}

! ---------------------------------------------------------------------------
! Manually execute an opcode. You don't have to use this. Use one of the
! helper routines above.
!
! Returns the status of the opcode (one of HGR_RESULT_*) and writes the
! opcode's output parameters to hugor_op_array.
!
! opcode: Opcode to execute.
! in_argc: Amount of input parameters the opcode expects.
! out_argc: Amount of output parameters the opcode returns.
! arg0..argN: Input parameters.
! ---------------------------------------------------------------------------
routine HugorOpcode(opcode, in_argc, out_argc, arg0, arg1, arg2, arg3, arg4,\
                    arg5, arg6)
{
    hugor_op_array[0] = arg0, arg1, arg2, arg3, arg4, arg5, arg6

    if (in_argc > HGR_MAX_OP_ARGS) {
        in_argc = HGR_MAX_OP_ARGS
    }
    if (out_argc > HGR_MAX_OP_ARGS) {
        out_argc = HGR_MAX_OP_ARGS
    }

    local i
    writefile HGR_CONTROL_FILE {
        writeval opcode
        for (i = 0; i < in_argc; ++i) {
            writeval hugor_op_array[i]
        }
    }

    local result
    readfile HGR_CONTROL_FILE {
        result = readval
        if (result ~= HGR_RESULT_OK) {
            return result
        }
        for (i = 0; i < out_argc; ++i) {
            hugor_op_array[i] = readval
        }
    }
    return result
}


! ---------------------------------------------------------------------------
! Opcode definitions.
! ---------------------------------------------------------------------------

! in:  none
! out: major
!      minor
!      patch
constant HGR_OP_GET_VERSION     100

! in:  none
! out: OS code (see HGR_OS_* constants)
constant HGR_OP_GET_OS          200

! in:  none
! out: none
constant HGR_OP_ABORT           300

! in:  block flag (true: block, false: asynchronous)
!      duration (milliseconds)
!      start opacity [-1..255]
!      end opacity [0..255]
! out: none
constant HGR_OP_FADE_SCREEN     400

! in:  URL
! out: none
constant HGR_OP_OPEN_URL        500

! in:  flag (true: fullscreen, false: windowed)
! out: none
constant HGR_OP_SET_FULLSCREEN  600

! in:  string
! out: none
constant HGR_OP_SET_CLIPBOARD   700

! ---------------------------------------------------------------------------
! Magic files.
! ---------------------------------------------------------------------------
constant HGR_CHECK_FILE "HrCheck"
constant HGR_CHECK_MAGIC 16962
constant HGR_CONTROL_FILE "HrCtlAPI"

#endif _HUGOROPC_H
