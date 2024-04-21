const form = document.getElementById("loginForm");

form.onsubmit = (evt) => {
    evt.preventDefault();

    const formData = new FormData(form);

    const user = formData.get("user");
    const pass = formData.get("pass");

    console.table({ user, pass })

    window.connection.login(user, pass);
}

window.connection.onLoginStatus((value) => {
    console.log(value);
})