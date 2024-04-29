const fs = require('fs');
const { ipcRenderer } = require('electron');

// Function to update the recipes text file
function updateRecipesFile(updatedRecipes) {
  const data = 'NAME QUANTITY IMPORTANCE DAY MONTH YEAR\n' + updatedRecipes.map(recipe => `${recipe.name} ${recipe.timeToCook} ${recipe.steps}`).join('\n');
  fs.writeFile('recipes.txt', data, err => {
    if (err) {
      console.error('Error writing to recipes.txt:', err);
    } else {
      console.log('Recipes file updated successfully.');
    }
  });
}

// Function to read the recipes file
/*
function readRecipesFile(callback) {
  fs.readFile('recipes.txt', 'utf8', (err, data) => {
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
*/
function readRecipesFile(callback) {
  fs.readFile('recipes.txt', 'utf8', (err, data) => {
    if (err) throw err;
    const lines = data.split('\n');
    const recipesData = [];
    let currentRecipe = null;

    lines.forEach((line, index) => {
      if (line.trim() === '') return; // Skip empty lines
      
      if (line.includes('====')) { // Check for start of a new recipe
        if (currentRecipe) {
          recipesData.push(currentRecipe); // Push the last completed recipe
        }
        console.log(line)
        const [name, timeToCook, steps] = line.split('====');
        console.log(steps)
        currentRecipe = { name, timeToCook, steps: [steps.trim()] }; // Start a new recipe
      } else if (currentRecipe) {
        currentRecipe.steps.push(line.trim()); // Continue adding steps to the current recipe
      }
    });

    if (currentRecipe) {
      recipesData.push(currentRecipe); // Push the last recipe if exists
    }
    
    callback(recipesData);
  });
}

document.getElementById('data').addEventListener('click', function() {
  console.log("Hello")
  const { populateRecipes } = require('./cpp-backend/build/Release/addon.node');

  // Calling functions of native addon
  var result = populateRecipes();
  console.log("I Ran");

  // Load existing recipes after populating
  readRecipesFile(function(recipesData) {
    renderRecipes(recipesData);
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
      const fullInstructions = recipe.steps.join('<br>'); // Join all steps with line breaks for HTML display
      openPopup(fullInstructions);
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