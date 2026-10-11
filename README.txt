Orion.Loader
============
C++20 scaffold producing a Windows x64 DLL on Windows or Linux with clang-cl.
Only Release and RelWithDebInfo are supported. The DLL exports initialize,
an alias for orion::Loader::initialize(const orion::InitializationContext&).
src/Exports.def maps the exported name directly to the C++ method; no wrapper
is needed. Update that alias if the method's signature or qualified name changes.
No loader behavior is implemented yet. InitializationContext and Macros.hpp come
from the Orion.Native submodule through its orion::NativeHeaders CMake target.

Clone with the public headers:

  git clone --recurse-submodules https://github.com/BedrockNative/Orion.Loader.git

For an existing checkout:

  git submodule update --init --recursive

Orion.Native lives in external/Orion.Native. Set
-DORION_NATIVE_ROOT=/path/to/Orion.Native to use another checkout.

Linux
-----
Requires CMake 3.25+, Ninja, LLVM clang-cl/lld-link, and an xwin SDK layout.
Use xwin (https://github.com/Jake-Shadle/xwin) to obtain the Microsoft SDK,
CRT and C++ standard library headers AND x64 libraries. Accept Microsoft's
license when prompted. With xwin installed:

  xwin --arch x86_64 splat --output /path/to/xwin
  export XWIN_ROOT=/path/to/xwin
  cmake --preset release
  cmake --build --preset release --parallel

For optimized code with debug symbols:

  cmake --preset relwithdebinfo
  cmake --build --preset relwithdebinfo --parallel

When XWIN_ROOT is unset, CMake searches $XDG_DATA_HOME/xwin,
~/.local/share/xwin, ~/.xwin and ~/xwin for an existing SDK. If none is found,
configuration stops with instructions to set XWIN_ROOT. No machine-specific
SDK path is stored in the project.
Alternatively pass -DXWIN_ROOT=/path/to/xwin to either configure command.
The SDK stays outside the repository. No Windows rc.exe or mt.exe is needed;
LLD embeds manifests directly. Paths containing spaces are supported.

Windows
-------
Install CMake 3.25+, Ninja, LLVM (clang-cl and lld-link), and Visual Studio
Build Tools with the C++ tools and Windows SDK. Open an x64 Native Tools
Command Prompt and ensure LLVM and Ninja are on PATH:

  cmake --preset release
  cmake --build --preset release --parallel

Or:

  cmake --preset relwithdebinfo
  cmake --build --preset relwithdebinfo --parallel

Windows uses the SDK/CRT/STL from the developer shell; xwin is only required
when cross-compiling from Linux. Both hosts target the Windows MSVC ABI.

Artifacts
---------
  build/<preset>/bin/Orion.Loader.dll
  build/<preset>/bin/Orion.Loader.pdb   (RelWithDebInfo)
  build/<preset>/lib/Orion.Loader.lib  (import library)

Both configurations use the static release CRT (/MT). RelWithDebInfo enables
optimization plus CodeView/PDB debug information; it does not use the debug CRT.
xwin may lack Microsoft's CRT source PDBs (LNK4099); your DLL's own symbols
are still generated.

Runtime-Importer
----------------
The root CMakeLists.txt includes toolchains/runtimeimporter/toolchain.cmake.
On configure it resolves the latest AmethystAPI/Runtime-Importer release and
fetches Runtime-Importer.zip, verifying its SHA-256 before extracting it.
Downloads are cached under build/<preset>/_tools/runtimeimporter/<tag>/
(for example manual-32/), with the archive stored alongside as <tag>.zip.

By default toolchains/runtimeimporter/toolchain.json contains {} (latest).
To pin a release, change it to:

  { "tag": "manual-38" }

An optional "sha256" field pins the archive digest too, and is required for
older releases without a GitHub asset digest. A cached pinned release can be
configured offline; latest requires GitHub access on each configure.

The module exposes RUNTIMEIMPORTER_TAG, RUNTIMEIMPORTER_DIR and
RUNTIMEIMPORTER_DLL (Amethyst.ModuleTweaker.dll) for subsequent build steps.
This only provisions the tool; DLL post-processing is not enabled yet.
Use -DRUNTIMEIMPORTER_CONFIG=/path/to/toolchain.json to override the selection,
or -DRUNTIMEIMPORTER_CACHE=/shared/cache to share downloads between builds.
