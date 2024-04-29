


// After make profile ->
document.addEventListener('DOMContentLoaded', function () {
document.getElementById('profile_submit').addEventListener('click', function() {
    event.preventDefault();
    let input1 = document.getElementById("username").value;
    let input2 = document.getElementById("birthday").value;

    // Getting checkbox values
    let restrictionsArray = [];
    let checkboxes = document.querySelectorAll('input[name="restrictions"]:checked');
    checkboxes.forEach(function(checkbox) {
        restrictionsArray.push(checkbox.value);
    });

    console.log(input1);
    console.log(input2);
    console.log(restrictionsArray);

    // send username, birthday, and restrictions to main.js 
    ipcRenderer.send('asynchronous-message', { 'input1': input1});

    // receive message from main.js
    ipcRenderer.on('asynchronous-reply', (event, arg) => {
        // Address of native addon
        const { masterProfile } = require('./cpp-backend/build/Release/addon.node');

        // Calling functions of native addon
        var result = masterProfile(arg['input1'], 10);

        document.getElementById('tag_result').innerHTML =
            "C++ Native addon add() result (IPC): " + result;
    });
});
})