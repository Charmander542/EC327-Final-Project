const fs = require('fs');

document.addEventListener('DOMContentLoaded', function () {
  const ingredientsList = document.getElementById('ingredients');
  let ingredientsData = [];

  // Load existing ingredients
  fs.readFile('ingredients.txt', 'utf8', (err, data) => {
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

    // Update the ingredients file
    updateIngredientsFile(ingredientsData);

    // Re-render the ingredients
    renderIngredients();
  });

  function renderIngredients() {
    let html = ''; // Initialize HTML string

    ingredientsData.forEach((ingredient, index) => {
      const { name, quantity, importance, day, month, year } = ingredient;

      // Create description
      const description = `Quantity: ${quantity}, Expiration Date: ${day}/${month}/${year}`;

      // Create HTML for ingredient
      html += `
        <div class="ingredient" draggable="true" data-index="${index}">
          <dt>${name}</dt>
          <dd>${description}</dd>
          <button class="delete-btn" data-name="${name}">X</button>
        </div>
      `;
    });

    // Set the HTML for the ingredients list
    ingredientsList.innerHTML = html;

    addDeleteListeners();
    addDragListeners();
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

  function addDragListeners() {
    const ingredients = document.querySelectorAll('.ingredient');
  
    ingredients.forEach(ingredient => {
      ingredient.addEventListener('dragstart', dragStart);
      ingredient.addEventListener('dragover', dragOver);
      ingredient.addEventListener('dragenter', dragEnter);
      ingredient.addEventListener('dragleave', dragLeave);
      ingredient.addEventListener('drop', drop);
      ingredient.addEventListener('dragend', dragEnd);
    });
  }
  
  let draggedElement = null; // Store the dragged element
  
  function dragStart(event) {
    draggedElement = event.target;
    event.dataTransfer.setData('text/plain', event.target.dataset.index);
  }
  
  function dragOver(event) {
    event.preventDefault();
  }
  
  function dragEnter(event) {
    event.preventDefault();
    if (event.target !== draggedElement) {
      event.target.classList.add('drop-target'); // Add drop-target class for visual feedback
    }
  }
  
  function dragLeave(event) {
    event.preventDefault();
    if (event.target !== draggedElement) {
      event.target.classList.remove('drop-target'); // Remove drop-target class when leaving drop area
    }
  }
  
  function drop(event) {
    event.preventDefault();
    const fromIndex = parseInt(event.dataTransfer.getData('text/plain'));
    const toIndex = parseInt(event.target.dataset.index);
  
    // Rearrange ingredientsData
    const movedIngredient = ingredientsData.splice(fromIndex, 1)[0];
    ingredientsData.splice(toIndex, 0, movedIngredient);
  
    // Update the ingredients file
    updateIngredientsFile(ingredientsData);
  
    // Re-render the ingredients
    renderIngredients();
  }
  
  function dragEnd(event) {
    event.preventDefault();
    // Remove drop-target class from all ingredients
    document.querySelectorAll('.ingredient').forEach(ingredient => {
      ingredient.classList.remove('drop-target');
    });
  }

  

  // Function to update the ingredients text file
  function updateIngredientsFile(updatedIngredients) {
    const data = 'NAME QUANTITY IMPORTANCE DAY MONTH YEAR\n' + updatedIngredients.map(ingredient => `${ingredient.name} ${ingredient.quantity} ${ingredient.importance} ${ingredient.day} ${ingredient.month} ${ingredient.year}`).join('\n');
    fs.writeFile('ingredients.txt', data, err => {
      if (err) {
        console.error('Error writing to ingredients.txt:', err);
      } else {
        console.log('Ingredients file updated successfully.');
      }
    });
  }
});
