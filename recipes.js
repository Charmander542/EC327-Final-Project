const fs = require('fs');
const { ipcRenderer } = require('electron');

// Function to update the recipes text file
function updateRecipesFile(updatedRecipes) {
  const data = 'NAME QUANTITY IMPORTANCE DAY MONTH YEAR\n' + updatedRecipes.map(recipe => `${recipe.name} ${recipe.timeToCook} ${recipe.steps}`).join('\n');
  fs.writeFile('./cpp-backend/src/recipes.txt', data, err => {
    if (err) {
      console.error('Error writing to recipes.txt:', err);
    } else {
      console.log('Recipes file updated successfully.');
    }
  });
}

// Function to read the recipes file
function readRecipesFile(callback) {
  fs.readFile('./cpp-backend/src/recipes.txt', 'utf8', (err, data) => {
    if (err) throw err;
    const lines = data.split('\n');
    const recipesData = [];
    lines.forEach((line, index) => {
  // Skip the header line
        const [name,timeToCook,steps] = line.split('~');
        recipesData.push({ name,timeToCook,steps });
    });
    callback(recipesData);
  });
}

document.getElementById('data').addEventListener('click', function() {
  // send username to main.js 
  ipcRenderer.send('asynchronous-message');

  // receive message from main.js
  ipcRenderer.on('asynchronous-reply', (event, arg) => {
    // Address of native addon
    const { populateRecipies } = require('./cpp-backend/build/Release/addon.node');

    // Calling functions of native addon
    var result = populateRecipies(); //TODO add a pass for the nonos

    document.getElementById('tag_result').innerHTML =
      "C++ Native addon populateRecipies() result (IPC): " + result;

    // Load existing recipes after populating
    readRecipesFile(function(recipesData) {
      renderRecipes(recipesData);
    });
  });
});

function renderRecipes(recipesData) {
  const recipesList = document.getElementById('recipes');
  recipesList.innerHTML = '';
  
  recipesData.forEach(recipe => {
    const { name,timeToCook,steps } = recipe;

    // Create description
    const description = `Time to Cook: ${timeToCook}`;

    // Create HTML for recipe item
    const recipeItem = document.createElement('li');
    recipeItem.className = 'recipe';
    recipeItem.innerHTML = `
      <h3>${name}</h3>
      <p>${description}</p>
    `;

    // Create button for recipe item
    const button = document.createElement('button');
    button.innerText = 'View Instructions';
    button.className = 'view-instructions-button';
    button.addEventListener('click', () => {
      // Read instructions from file and open popup
      openPopup(steps);
    });

    // Append button to recipe item
    recipeItem.appendChild(button);

    recipesList.appendChild(recipeItem);
  });
}

function openPopup(instructions) {
  // Create a popup element
  const popup = document.createElement('div');
  popup.className = 'popup';
  popup.innerHTML = `
    <div class="popup-content">
      <span class="close-popup">&times;</span>
      <h2>Recipe Instructions</h2>
      <p>${instructions}</p>
    </div>
  `;

  // Close popup when close button is clicked
  const closeButton = popup.querySelector('.close-popup');
  closeButton.addEventListener('click', () => {
    document.body.removeChild(popup);
  });

  // Append the popup to the body
  document.body.appendChild(popup);
}