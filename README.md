# WayCrateLock

session-lock-v1 with qml

## Toy project -- till now
Support mpris, current only works on sway

Base on [qt-session-lock](https://github.com/waycrate/qt-session-lock)

## Config

WayCrateLock reads configuration from `$XDG_CONFIG_HOME/waycratelock/setting.toml` file ($XDG_CONFIG_HOME is typically `$HOME/.config/` for most Linux systems)

Here is an example TOML file [setting.toml](./assets/config/setting.toml)

## Help needed

The password text field only can input on the first screen registered, I think the problem is in qt-session-lock, the qt binding for session-lock-v1, but I cannot handle it.

## View
![Show](images/shown.png)
