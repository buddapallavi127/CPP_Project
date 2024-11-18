document.getElementById('fetchData').addEventListener('click', function() {
    fetch('http://localhost:18080/')
        .then(response => response.text())
        .then(data => {
            document.getElementById('response').innerText = data;
        });
});
