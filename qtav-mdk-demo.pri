
BUILD_TYPE=0
CONFIG(MinGW, MinGW|MinGW32|MinGW64|MSVC|MSVC32|MSVC64):{
    BUILD_TYPE = 1
}
else:CONFIG(MinGW64, MinGW|MinGW32|MinGW64|MSVC|MSVC32|MSVC64):{
    BUILD_TYPE = 1
}
else:CONFIG(MSVC64, MinGW|MinGW32|MinGW64|MSVC|MSVC32|MSVC64):{
    BUILD_TYPE = 1
}

equals(BUILD_TYPE, 0) {
# 这里根据实际使用的编译器进行替换 MinGW, MinGW64, MSVC
CONFIG *= MinGW64
}

MDK_SDK_PATH=$$PWD/mdk-sdk

# Config Para
CONFIG(debug, debug|release):{
        FILE_POSTFIX = D
        DIR_COMPILEMODE = Debug
}
else:CONFIG(release, debug|release):{
        FILE_POSTFIX =
        DIR_COMPILEMODE = Release
}

win32:{
        CONFIG(MinGW, MinGW|MinGW32|MinGW64|MSVC|MSVC32|MSVC64):{
                DIR_COMPILER = MinGW
                DIR_PLATFORM = Win32
                FILE_LIB_PREFIX = lib
                FILE_LIB_EXT = .a
        }
        else:CONFIG(MinGW64, MinGW|MinGW32|MinGW64|MSVC|MSVC32|MSVC64):{
                DIR_COMPILER = MinGW
                DIR_PLATFORM = Win64
                FILE_LIB_PREFIX =lib
                FILE_LIB_EXT = .a
        }
        else:CONFIG(MSVC64, MinGW|MinGW32|MinGW64|MSVC|MSVC32|MSVC64):{
                DIR_COMPILER = MSVC
                DIR_PLATFORM = Win64
                FILE_LIB_PREFIX =
                FILE_LIB_EXT = .lib
        }

        FILE_DLL_PREFIX =
        FILE_DLL_EXT = .dll
}
else:android:{
        CONFIG(ARM_GCC_4.4.3, ARM_GCC_4.4.3|ARM_GCC_4.6|ARM_GCC_4.7|ARM_GCC_4.8):{
                DIR_COMPILER = ARM_GCC_4.4.3
        }
        else:CONFIG(ARM_GCC_4.6, ARM_GCC_4.4.3|ARM_GCC_4.6|ARM_GCC_4.7|ARM_GCC_4.8):{
                DIR_COMPILER = ARM_GCC_4.6
        }
        else:CONFIG(ARM_GCC_4.7, ARM_GCC_4.4.3|ARM_GCC_4.6|ARM_GCC_4.7|ARM_GCC_4.8):{
                DIR_COMPILER = ARM_GCC_4.7
        }
        else:CONFIG(ARM_GCC_4.8, ARM_GCC_4.4.3|ARM_GCC_4.6|ARM_GCC_4.7|ARM_GCC_4.8):{
                DIR_COMPILER = ARM_GCC_4.8
        }

        CONFIG(X86_GCC_4.4.3, X86_GCC_4.4.3|X86_GCC_4.6|X86_GCC_4.7|X86_GCC_4.8):{
                DIR_COMPILER = X86_GCC_4.4.3
        }
        else:CONFIG(X86_GCC_4.6, X86_GCC_4.4.3|X86_GCC_4.6|X86_GCC_4.7|X86_GCC_4.8):{
                DIR_COMPILER = X86_GCC_4.6
        }
        else:CONFIG(X86_GCC_4.7, X86_GCC_4.4.3|X86_GCC_4.6|X86_GCC_4.7|X86_GCC_4.8):{
                DIR_COMPILER = X86_GCC_4.7
        }
        else:CONFIG(X86_GCC_4.8, X86_GCC_4.4.3|X86_GCC_4.6|X86_GCC_4.7|X86_GCC_4.8):{
                DIR_COMPILER = X86_GCC_4.8
        }

        DIR_PLATFORM = Android
        FILE_LIB_PREFIX = lib
        FILE_LIB_EXT = .a
        FILE_DLL_PREFIX = lib
        FILE_DLL_EXT = .so
}
else:ios:{
        CONFIG(LLVM, LLVM):{
                DIR_COMPILER = LLVM
        }

        DEFINES += IOS
        DIR_PLATFORM = IOS
        FILE_LIB_PREFIX = lib
        FILE_LIB_EXT = .a
        FILE_DLL_PREFIX = lib
        FILE_DLL_EXT = .so
}
else:mac:{
        CONFIG(clang, clang):{
                DIR_COMPILER = clang
        }

        DEFINES += MAC
        DIR_PLATFORM = MAC
        FILE_LIB_PREFIX = lib
        FILE_LIB_EXT = .a
        FILE_DLL_PREFIX = lib
        FILE_DLL_EXT = .so
}
else:linux:{
        CONFIG(GCC, GCC|GCC32|GCC64):{
                        DIR_COMPILER = GCC32
                        DIR_PLATFORM = Linux32
        }
        else:CONFIG(GCC32, GCC|GCC32|GCC64):{
                        DIR_COMPILER = GCC32
                        DIR_PLATFORM = Linux32
        }
        else:CONFIG(GCC64, GCC|GCC32|GCC64):{
                        DIR_COMPILER = GCC64
                        DIR_PLATFORM = Linux64
        }

        FILE_LIB_PREFIX = lib
        FILE_LIB_EXT = .a
        FILE_DLL_PREFIX = lib
        FILE_DLL_EXT = .so
}

CONFIG(USE_LIBRARY_ABN):{
        FILE_ABN_PREFIX = $${FILE_LIB_PREFIX}
        FILE_ABN_EXT = $${FILE_LIB_EXT}
}
else:{
        FILE_ABN_PREFIX = $${FILE_DLL_PREFIX}
        FILE_ABN_EXT = $${FILE_DLL_EXT}
}

DESTDIR	= $$PWD/bin/$${DIR_PLATFORM}/$${DIR_COMPILER}/$${DIR_COMPILEMODE}/

DIR_DEPEND_DEST = $$PWD/bin/$${DIR_PLATFORM}/$${DIR_COMPILER}/$${DIR_COMPILEMODE}
