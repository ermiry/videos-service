# Videos Upload Test Service

### Development
```
sudo docker run \
  -it \
  --name videos --rm \
  -p 5001:5001 --net videos \
  -v /home/ermiry/Documents/ermiry/Projects/videos-server/videos-service:/home/videos \
  -v /home/ermiry/Documents/ermiry/Projects/videos-server/uploads:/home/uploads \
  -e RUNTIME=development \
  -e PORT=5001 \
  -e CERVER_RECEIVE_BUFFER_SIZE=16384 -e CERVER_TH_THREADS=4 \
  -e CERVER_CONNECTION_QUEUE=4 \
  -e MONGO_APP_NAME=upload -e MONGO_DB=videos \
  -e MONGO_URI=mongodb://upload:password@mongo:27017/videos \
  ermiry/videos-service:development /bin/bash
```

### Test
```
sudo docker run \
  -it \
  --name videos --rm \
  -p 5001:5001 --net videos \
  -v /home/ermiry/Documents/ermiry/Projects/videos-server/uploads:/home/uploads \
  -e RUNTIME=test \
  -e PORT=5001 \
  -e CERVER_RECEIVE_BUFFER_SIZE=16384 -e CERVER_TH_THREADS=4 \
  -e CERVER_CONNECTION_QUEUE=4 \
  -e MONGO_APP_NAME=upload -e MONGO_DB=videos \
  -e MONGO_URI=mongodb://upload:password@mongo:27017/videos \
  ermiry/videos-service:test /bin/bash
```

## Routes

### Main

#### GET /api/videos
**Description:** Videos service top level route \
**Returns:**
  - 200 on success

#### GET /api/videos/version
**Access:** Public \
**Description:** Returns videos service current version \
**Returns:**
  - 200 and version's json on success

### Service

#### POST /api/videos/upload
**Access:** Public \
**Description:** Uploads a video to the service \
**Returns:**
  - 200 on success
  - 400 on bad request
  - 500 on server error

#### POST /api/videos/upload/complete
**Access:** Public \
**Description:** Signals that an upload has finished \
**Returns:**
  - 200 on success
  - 400 on bad request
  - 500 on server error
