TC_ENV :=
TC_ENV += LD="/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/bin/x86_64-pc-linux-gnu-ld"
TC_ENV += LDSHARED="/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/bin/x86_64-pc-linux-gnu-gcc -shared"
TC_ENV += CPP="/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/bin/x86_64-pc-linux-gnu-cpp"
TC_ENV += NM="/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/bin/x86_64-pc-linux-gnu-nm"
TC_ENV += CC="/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/bin/x86_64-pc-linux-gnu-gcc"
TC_ENV += AS="/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/bin/x86_64-pc-linux-gnu-as"
TC_ENV += RANLIB="/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/bin/x86_64-pc-linux-gnu-ranlib"
TC_ENV += CXX="/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/bin/x86_64-pc-linux-gnu-g++"
TC_ENV += AR="/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/bin/x86_64-pc-linux-gnu-ar"
TC_ENV += STRIP="/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/bin/x86_64-pc-linux-gnu-strip"
TC_ENV += OBJDUMP="/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/bin/x86_64-pc-linux-gnu-objdump"
TC_ENV += READELF="/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/bin/x86_64-pc-linux-gnu-readelf"
TC_ENV += CFLAGS="-I/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/x86_64-pc-linux-gnu/sys-root/usr/include -I/spksrc/spk/zsh/work-bromolow-7.0/install//var/packages/zsh/target/include $(ADDITIONAL_CFLAGS)"
TC_ENV += CPPFLAGS="-I/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/x86_64-pc-linux-gnu/sys-root/usr/include -I/spksrc/spk/zsh/work-bromolow-7.0/install//var/packages/zsh/target/include $(ADDITIONAL_CPPFLAGS)"
TC_ENV += CXXFLAGS="-I/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/x86_64-pc-linux-gnu/sys-root/usr/include -I/spksrc/spk/zsh/work-bromolow-7.0/install//var/packages/zsh/target/include $(ADDITIONAL_CXXFLAGS)"
TC_ENV += LDFLAGS="-L/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/x86_64-pc-linux-gnu/sys-root/lib -L/spksrc/spk/zsh/work-bromolow-7.0/install//var/packages/zsh/target/lib -Wl,--rpath-link,/spksrc/spk/zsh/work-bromolow-7.0/install//var/packages/zsh/target/lib -Wl,--rpath,/var/packages/zsh/target/lib $(ADDITIONAL_LDFLAGS)"
TC_CONFIGURE_ARGS := --host=x86_64-pc-linux-gnu --build=i686-pc-linux
TC_TYPE := DSM
TC_TARGET := x86_64-pc-linux-gnu
TC_PREFIX := x86_64-pc-linux-gnu-
TC_PATH := /spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/bin/
CFLAGS := -I/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/x86_64-pc-linux-gnu/sys-root/usr/include -I/spksrc/spk/zsh/work-bromolow-7.0/install//var/packages/zsh/target/include $(ADDITIONAL_CFLAGS)
CPPFLAGS := -I/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/x86_64-pc-linux-gnu/sys-root/usr/include -I/spksrc/spk/zsh/work-bromolow-7.0/install//var/packages/zsh/target/include $(ADDITIONAL_CPPFLAGS)
CXXFLAGS := -I/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/x86_64-pc-linux-gnu/sys-root/usr/include -I/spksrc/spk/zsh/work-bromolow-7.0/install//var/packages/zsh/target/include $(ADDITIONAL_CXXFLAGS)
LDFLAGS := -L/spksrc/toolchain/syno-bromolow-7.0/work/x86_64-pc-linux-gnu/x86_64-pc-linux-gnu/sys-root/lib -L/spksrc/spk/zsh/work-bromolow-7.0/install//var/packages/zsh/target/lib -Wl,--rpath-link,/spksrc/spk/zsh/work-bromolow-7.0/install//var/packages/zsh/target/lib -Wl,--rpath,/var/packages/zsh/target/lib $(ADDITIONAL_LDFLAGS)
TC_LIBRARY := x86_64-pc-linux-gnu/sys-root/lib
TC_INCLUDE := x86_64-pc-linux-gnu/sys-root/usr/include
TC_EXTRA_CFLAGS :=
TC_VERS := 7.0
TC_BUILD := 40000
TC_OS_MIN_VER := 7.0-40000
TC_ARCH := bromolow
TC_KERNEL := 3.10.108
TC_GCC := 7.3.0
TC_GLIBC := 2.26
