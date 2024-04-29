## Requirements
- [Node.js](https://nodejs.org/en/download/) (>= 10.x)
- [CMake](https://cmake.org/download/) (>= 3.15)
- [Electron](https://www.electronjs.org/docs/latest/tutorial/installation) (lastest)

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

## TODO

- Profile: right now we have load profile that will return a list of nonos but it will be easier if the profile page will just place the chosen user at the top of the users.txt so that in the recipes page we can call loadLastProfile and the function will auto return the nonos for the findRecipes function.
- Recipes: Right now we make a request to find recipes but as it stands we don't have any of the files we need and there doens't seem to be logic to properly load a recipe.
- We need to edit recipes.html page so you can click on a given recipe and laod the instructions in a new tab or something.
