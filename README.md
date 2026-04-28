# TimerApp
This is a very simple timer app made in C++ with Qt.


## Adding timers
The only way to add timers is to edit `timers.txt` in your local app data folder, after running the program once. I tried so many methods to make removing timers and dynamically moving them into new positions work, but I did not succeed.

The easiest way to find `timers.txt` is to type `"%LOCALAPPDATA%/TimerApp/timers.txt"` into either the address bar in File Explorer or Command Prompt.

I'll eventually add a way to do it in-app, but the constant segmentation errors my previous approaches gave me were super demotivating.