const fs = require('fs');

// Function to update the ingredients JSON file
function updateIngredientsFile(updatedIngredients) {
  fs.writeFile('ingredients.json', JSON.stringify({ ingredients: updatedIngredients }, null, 2), err => {
    if (err) {
      console.error('Error writing to ingredients.json:', err);
    } else {
      console.log('Ingredients file updated successfully.');
    }
  });
}

document.addEventListener('DOMContentLoaded', function () {
  const ingredientsList = document.getElementById('ingredients');
  let ingredientsData = null;

  // Load existing ingredients
  fs.readFile('ingredients.json', (err, data) => {
    if (err) throw err;
    ingredientsData = JSON.parse(data);
    renderIngredients();
  });

  // Access the submit button and add event listener
  document.getElementById('ingredient_submit').addEventListener('click', function () {
    var date = new Date(document.getElementById("ingredient_date").value);
    var day = date.getDate();
    var month = date.getMonth() + 1; // JavaScript months are 0-11
    var year = date.getFullYear();

    const newIngredient = {
      name: document.getElementById("ingredient_name").value,
      quantity: document.getElementById("ingredient_quantity").value,
      unit: document.getElementById("ingredient_unit").value,
      day: day,
      month: month,
      year: year
    };

    // Add the new ingredient to the list
    ingredientsData.ingredients.push(newIngredient);

    // Update the ingredients file
    updateIngredientsFile(ingredientsData.ingredients);

    // Re-render the ingredients
    renderIngredients();
  });

  function renderIngredients() {
    let html = ''; // Initialize HTML string

    ingredientsData.ingredients.forEach(ingredient => {
      const { name, quantity, unit } = ingredient;

      // Create description
      const description = `${quantity} ${unit}`;

      // Create HTML for ingredient
      html += `
        <div class="ingredient">
          <dt>${name}</dt>
          <dd>${description}</dd>
          <button class="delete-btn" data-name="${name}">X</button>
        </div>
      `;
    });

    // Set the HTML for the ingredients list
    ingredientsList.innerHTML = html;

    addDeleteListeners();
  }

  function addDeleteListeners() {
    const deleteButtons = document.querySelectorAll('.delete-btn');
    deleteButtons.forEach(button => {
      button.addEventListener('click', event => {
        const ingredientName = event.target.getAttribute('data-name');

        // Remove the ingredient from the list
        ingredientsData.ingredients = ingredientsData.ingredients.filter(ingredient => ingredient.name !== ingredientName);

        // Update the ingredients file
        updateIngredientsFile(ingredientsData.ingredients);

        // Re-render the ingredients
        renderIngredients();
      });
    });
  }
});






//add a new ingredient
document.getElementById('ingredient_submit').addEventListener('click', function () {
  // initializing a JavaScript object
  /*const new_ingredient = {
    name: document.getElementById("ingredient_name").value,
    quantity: document.getElementById("ingredient_quantity").value,
    unit: document.getElementById("ingredient_unit").value,
    date: document.getElementById("ingredient_expiration").value
  };
  var ingredientJSON = JSON.stringify(new_ingredient);
  */

  var date = new Date(document.getElementById("ingredient_date").value);
  var day = date.getDay();
  var month = date.getMonth();
  var year = date.getFullYear();

  //add the ingredient to the list
  //ingredient(document.getElementById("ingredient_name").value, document.getElementById("ingredient_quantity").value, document.getElementById("ingredient_date").value);

  // writing the JSON string content to a file
  //fs.writeFile("ingredients.json", ingredientJson, (error) => {
  // throwing the error
  // in case of a writing problem
  //if (error) {
  // logging the error
  //console.error(error);

  //throw error;
  // }

  //console.log("ingredients.json written correctly");
  //});
});

// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// No Node.js APIs are available in this process because
// `nodeIntegration` is turned off. Use `preload.js` to
// selectively enable features needed in the rendering
// process.

// Wait for the DOM to finish loading
document.addEventListener('DOMContentLoaded', function () {

  // Access the submit button and add event listener
  document.getElementById('ingredient_submit').addEventListener('click', function () {

    const ingredient = {
      name: "Banana",
      quantity: "99",
      importance: "10",
      day: 26,
      month: 4,
      year: 2024
    };

    console.log("Adding ingredient: " + ingredient)

    // send ingredient data to main.js 
    ipcRenderer.send('add-ingredient', ingredient);

    // receive message from main.js
    ipcRenderer.on('add-ingredient-result', (event, arg) => {
      console.log(arg);
    });
  });

});