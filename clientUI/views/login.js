const form = document.getElementById("loginForm");
const login = document.getElementById("loginBtn");
const signup = document.getElementById("signupBtn");

form.onsubmit = (evt) => evt.preventDefault();

signup.addEventListener("click", () => {
    const formData = new FormData(form);

    const user = formData.get("user");
    const pass = formData.get("pass");

    console.table({ user, pass })

    window.connection.signup(user, pass);
})

login.addEventListener("click", () => {
    const formData = new FormData(form);

    const user = formData.get("user");
    const pass = formData.get("pass");

    console.table({ user, pass })

    window.connection.login(user, pass);
})


window.connection.onLoginStatus((value) => {
    if (value == "success") {
        window.location.replace("./game.html")
    }
})