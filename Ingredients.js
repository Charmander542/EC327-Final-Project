const fs = require('fs');

// Function to update the ingredients text file
function updateIngredientsFile(updatedIngredients) {
  const data = 'NAME QUANTITY IMPORTANCE DAY MONTH YEAR\n' + updatedIngredients.map(ingredient => `${ingredient.name} ${ingredient.quantity} ${ingredient.importance} ${ingredient.day} ${ingredient.month} ${ingredient.year}`).join('\n');
  fs.writeFile('./cpp-backend/src/ingredients.txt', data, err => {
    if (err) {
      console.error('Error writing to ingredients.txt:', err);
    } else {
      console.log('Ingredients file updated successfully.');
    }
  });
}

document.addEventListener('DOMContentLoaded', function () {
  const ingredientsList = document.getElementById('ingredients');
  let ingredientsData = [];

  // Load existing ingredients
  fs.readFile('./cpp-backend/src/ingredients.txt', 'utf8', (err, data) => {
    if (err) throw err;
    const lines = data.split('\n');
    lines.forEach((line, index) => {
      if (index !== 0) { // Skip the header line
        const [name, quantity, importance, day, month, year] = line.split(' ');
        ingredientsData.push({ name, quantity, importance, day, month, year });
      }
    });
    renderIngredients();
  });

  // Access the submit button and add event listener
  document.getElementById('ingredient_submit').addEventListener('click', function () {
    event.preventDefault();
    var date = new Date(document.getElementById("ingredient_date").value);
    var day = date.getDate();
    var month = date.getMonth() + 1; // JavaScript months are 0-11
    var year = date.getFullYear();

    const newIngredient = {
      name: document.getElementById("ingredient_name").value,
      quantity: document.getElementById("ingredient_quantity").value,
      importance: 1,
      day: day,
      month: month,
      year: year
    };

    // Add the new ingredient to the list
    ingredientsData.push(newIngredient);
    //console.log(ingredientsData);

    // Update the ingredients file
    updateIngredientsFile(ingredientsData);

    // Re-render the ingredients
    renderIngredients();
  });

  function renderIngredients() {
    let html = ''; // Initialize HTML string

    ingredientsData.forEach(ingredient => {
      const { name, quantity, importance, day, month, year } = ingredient;

      // Create description
      const description = `${quantity}, Importance: ${importance}, Expiration Date: ${day}/${month}/${year}`;

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
        ingredientsData = ingredientsData.filter(ingredient => ingredient.name !== ingredientName);

        // Update the ingredients file
        updateIngredientsFile(ingredientsData);

        // Re-render the ingredients
        renderIngredients();
      });
    });
  }
});
