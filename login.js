$(document).ready(function() {
    $(".dropdown").click(function(event) {
        event.preventDefault(); // Prevent the default link behavior
        $(".list a").toggle(); // Toggle visibility of the paragraphs in the list
    });
});

// AJAX function to submit the login form
function submitLoginForm() {
    var username = document.getElementById('sign_in').value;
    var password = prompt('Enter password:');  // Prompt user for password

    if (!username || !password) {
        alert('Please provide both username and password!');
        return;
    }

    // Send the data to the backend
    $.ajax({
        url: 'http://localhost:8080/login',  // URL of your C++ backend API
        type: 'POST',
        data: {
            username: username,
            password: password
        },
        success: function(response) {
            if (response.status === 'success') {
                alert('Sign-in successful!');
            } else {
                alert('Invalid credentials!');
            }
        },
        error: function(xhr, status, error) {
            alert('An error occurred during the sign-in process.');
        }
    });
}