## General
- Added dedicated video chunk definition
- Added new video files merge method that uses sendfile for a zero copy implementation
- Split merge files into multiple methods
- Stopping merge if there is a problem with one of the chunks