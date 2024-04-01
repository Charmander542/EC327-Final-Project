## Requirements
- [Node.js](https://nodejs.org/en/download/) (>= 10.x)
- [CMake](https://cmake.org/download/) (>= 3.15)

## To deploy the application
```bash
# Go into the backend folder
cd cpp-backend
# Run Instal
npm install
# Move back to the main folder
cd ..
# Install the dependencies if not already done
npm install
# Run the code
npm start
```

## Devlopment

- Frontend: edit the html and javascript to build up a front end and use the renderer to make calls to the backend
- Backend: add cpp and h files to the folder called src in the cpp-backend folder, follow the example but it works kind of like a namespace and you need to add the functions to the Napi exports
