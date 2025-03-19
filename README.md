# WayCrateLock

session-lock-v1 with qml

## Toy project -- till now
Support mpris, current only works on sway

Base on [qt-session-lock](https://github.com/waycrate/qt-session-lock)

## Config

WayCrateLock reads configuration from `$XDG_CONFIG_HOME/waycratelock/setting.toml` file ($XDG_CONFIG_HOME is typically `$HOME/.config/` for most Linux systems)

Here is an example TOML file [setting.toml](./assets/config/setting.toml)

## Building

This project uses a Makefile (wrapper for CMake) that can build targets like so:

```bash
make # will setup pam and build WayCrateLock locally

make install # for a global install (system wide)
```

> [!NOTE]
> 
> In case you want to manually build **without** Makefile,
> 
> Ensure that `misc/waycratelock` file is copied over to your `/etc/pam.d/waycratelock`
> 
> Without this, the screenlock will **NEVER** unlock as PAM will continue to fail authenticate you!
> 

**Building using CMake:**

```bash
sudo cp misc/waycratelock /etc/pam.d/waycratelock # ENSURE THAT THIS FILE EXISTS IN /etc/pam.d!!

cmake -S . -B build 
cmake --build build/

./build/waycratelock # to run the binary
```

## Help needed

The password text field only can input on the first screen registered, I think the problem is in qt-session-lock, the qt binding for session-lock-v1, but I cannot handle it.

## View
![Show](images/shown.png)
