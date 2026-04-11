# Repository Guidelines

## Project Structure & Module Organization
`src/` contains the C++20 application code. Major areas include `src/simulation/` for physics and element behavior, `src/gui/` for UI, `src/client/` for saves/networking, `src/common/` for shared utilities, and `src/lua/` for scripting support. Individual elements live in `src/simulation/elements/*.cpp`. Static assets and packaging metadata are under `resources/`, Android-specific files are in `android/`, and Meson build definitions are split across `meson.build` files throughout the tree.

## Build, Test, and Development Commands
Use Meson/Ninja for local development:

- `meson setup build` initializes a default build directory.
- `meson compile -C build` builds the main desktop executable.
- `meson setup build-clangtidy -Dclang_tidy=true` configures a lint-enabled build.
- `meson compile -C build-clangtidy clang-tidy` runs the repository’s `run-clang-tidy` target.
- `meson configure build` reviews active options such as `-Dbuild_render=true` or `-Dbuild_font=true`.

If you already use the checked-in `build-release/` directory, prefer reconfiguring it rather than creating ad hoc scripts.

## Coding Style & Naming Conventions
Follow the surrounding C++ style exactly: tabs for indentation, opening braces on a new line, and include blocks grouped similarly to nearby files. Use `PascalCase` for types and many methods, `camelCase` for local variables, and uppercase identifiers for element names like `PT_WATR`. Keep new files and symbols consistent with existing subsystem naming, especially under `src/simulation/elements/`.

Use `.clang-tidy` for static analysis guidance, but do not mass-reformat unrelated code.

## Testing Guidelines
There is no prominent standalone unit-test suite in this repository. Validate changes by compiling successfully and doing targeted runtime checks in the affected area. For example, element changes should be exercised in-game, while UI changes should be checked in the relevant screen or dialog. Mention any manual verification steps in your PR.

## Commit & Pull Request Guidelines
Recent history uses short, imperative commit subjects such as `Add api for controlling textbox focus` and `Appease clang-tidy`. Keep commits focused and avoid mixing refactors with behavior changes.

Pull requests should include a concise description, the reason for the change, and manual test notes. Link related issues when applicable, and add screenshots or short recordings for visible UI changes.
