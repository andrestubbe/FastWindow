# Building FastWindow from Source

## Prerequisites

- **JDK 17+** — [Download](https://adoptium.net/)
- **Maven 3.9+** — [Download](https://maven.apache.org/download.cgi)
- **Visual Studio 2022** — Community/Professional/Enterprise/BuildTools

## Quick Build

```bash
# 1. Build native DLL first (Windows)
compile.bat

# 2. Build JAR
mvn clean package -DskipTests
```

## Native DLL Build

The `compile.bat` script:
- Auto-detects Visual Studio Build Tools.
- Links `jawt.lib` and `dwmapi.lib`.
- Uses `native\FastWindow.def` for JNI exports.
- Outputs to `src/main/resources/native/fastwindow.dll`.

## Troubleshooting

**"Cannot find DLL"** — Run `compile.bat` first.

**"UnsatisfiedLinkError"** — Ensure `FastWindow.def` contains all exported function names.
