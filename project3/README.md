# Project 3

## Description
Implement Stopwatch device driver using FND and keys  
Used interrupt (top half, bottom half) for keys  
Used kernel timer

## Usage
device driver name: "/dev/stopwatch"  
device driver major number: 242  

Enter followings at device
```bash
$ insmod stopwatch.ko
$ mknod /dev/stopwatch c 242 0
$ ./app
```

## Implementation
### FND
Show minutes and seconds for 2 digits each.  Initialized 0000.

### Home key
Start stopwatch.

### Back key
Pause stopwatch. When restarting, the moment when paused should be considered. (ex. If paused at 4.5 seconds, stopwatch should change to 5 after 0.5 seconds when restarted.)

### Vol+ key
Reset stopwatch.

### Vol- key
Stop stopwatch when pressed for 3 seconds. app should exit only when stopwatch stopped.

### app
app is the test application for stopwatch  
app sleeps once it calls open and calls close when woken up