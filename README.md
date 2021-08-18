# Videos Upload Test Service

### Development
```
sudo docker run \
  -it \
  --name videos --rm \
  -p 5001:5001 \
  -v /home/ermiry/Documents/ermiry/Projects/videos-service:/home/videos \
  -e RUNTIME=development \
  -e PORT=5001 \
  -e CERVER_RECEIVE_BUFFER_SIZE=4096 -e CERVER_TH_THREADS=4 \
  -e CERVER_CONNECTION_QUEUE=4 \
  ermiry/videos-service:development /bin/bash
```

## Routes

### Main

#### GET /videos
**Description:** Videos service top level route \
**Returns:**
  - 200 on success

#### GET /videos/version
**Access:** Public \
**Description:** Returns videos service current version \
**Returns:**
  - 200 and version's json on success
