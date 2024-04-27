const fs = require('fs');

// Function to update the recipes text file
function updateRecipesFile(updatedRecipes) {
  const data = 'NAME QUANTITY IMPORTANCE DAY MONTH YEAR\n' + updatedRecipes.map(recipe => `${recipe.name} ${recipe.quantity} ${recipe.importance} ${recipe.day} ${recipe.month} ${recipe.year}`).join('\n');
  fs.writeFile('.cpp-backend/src/recipes.txt', data, err => {
    if (err) {
      console.error('Error writing to recipes.txt:', err);
    } else {
      console.log('Recipes file updated successfully.');
    }
  });
}

// Function to read the recipes file
function readRecipesFile(callback) {
  fs.readFile('.cpp-backend/src/recipes.txt', 'utf8', (err, data) => {
    if (err) throw err;
    const lines = data.split('\n');
    const recipesData = [];
    lines.forEach((line, index) => {
      if (index !== 0) { // Skip the header line
        const [name, quantity, importance, day, month, year] = line.split(' ');
        recipesData.push({ name, quantity, importance, day, month, year });
      }
    });
    callback(recipesData);
  });
}

document.addEventListener('DOMContentLoaded', function () {
  const recipesList = document.getElementById('recipes');

  // Load existing recipes
  readRecipesFile(function(recipesData) {
    renderRecipes(recipesData);
  });

  function renderRecipes(recipesData) {
    let html = ''; // Initialize HTML string

    recipesData.forEach(recipe => {
      const { name, quantity, importance, day, month, year } = recipe;

      // Create description
      const description = `${quantity}, Importance: ${importance}, Expiration Date: ${day}/${month}/${year}`;

      // Create HTML for recipe
      html += `
        <li class="recipe">
          <h3>${name}</h3>
          <p>${description}</p>
          <button class="delete-btn" data-name="${name}">X</button>
        </li>
      `;
    });

    // Set the HTML for the recipes list
    recipesList.innerHTML = html;

    addDeleteListeners(recipesData);
  }

  function addDeleteListeners(recipesData) {
    const deleteButtons = document.querySelectorAll('.delete-btn');
    deleteButtons.forEach(button => {
      button.addEventListener('click', event => {
        const recipeName = event.target.getAttribute('data-name');

        // Remove the recipe from the list
        recipesData = recipesData.filter(recipe => recipe.name !== recipeName);

        // Update the recipes file
        updateRecipesFile(recipesData);

        // Re-render the recipes
        renderRecipes(recipesData);
      });
    });
  }
});
