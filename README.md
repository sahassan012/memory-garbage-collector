# A Memory Garbage Collector
## About

## How it works
```console
----------------------------------------------------------------------
|h|x|x|f|h| | | | |f|h|x|x|x|x|f|h| | | | |...
----------------------------------------------------------------------
  ^       ^           ^           ^
  |       |           |           |
  a       b           c           the rest of the heap...
  (inuse) (free)      (inuse) 
```

## How to run (from command line)
