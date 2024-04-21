const form = document.getElementById("loginForm");
const login = document.getElementById("loginBtn");
const signup = document.getElementById("signupBtn");
const remember = document.getElementById("remember");
const dialog = document.querySelector("div.dialog");
const passField = document.getElementById("pass");
const userField = document.getElementById("user");
const dialogTitle = document.querySelector("h3.dialog-title");
const dialogInfo = document.querySelector("p.dialog-info");

window.onload = () => {
    const loginInfo = localStorage.getItem("login-info");
    if (!loginInfo) return;

    const { username, password } = JSON.parse(loginInfo);
    passField.value = password;
    userField.value = username;
    remember.checked = true;
}

form.onsubmit = (evt) => evt.preventDefault();

let timeout = null;
let formData = null;

function cancelDialog() {
    if (timeout === null) return;
    clearTimeout(timeout);
    dialog.className = "dialog hidden";
}

function showDialog(title, text, style) {
    dialog.className = `dialog ${style}`;
    dialogTitle.innerHTML = `${title}`
    dialogInfo.innerHTML = `${text}`;
    closeDialog(3000);
}

signup.addEventListener("click", () => {
    cancelDialog();
    formData = new FormData(form);
    const user = formData.get("user").toString();
    const pass = formData.get("pass").toString();

    if (user.trim() === "" || pass.trim() === "") {
        showDialog("Couldn't create an account", "There are missing fields to create an account", "error");
        return;
    }

    const fieldFormat = new RegExp("^[a-zA-Z0-9_]*$");
    if (!(fieldFormat.test(user) && fieldFormat.test(pass))) {
        showDialog("Couldn't create an account", "The fields should only by alphanumeric character or underscores", "error");
        return;
    }

    if (user.length < 8 || pass.length < 8) {
        showDialog("Couldn't create an account", "the fields should be at least 8 characters long", "error");
        return;
    }

    window.connection.signup(user, pass);
})

login.addEventListener("click", () => {
    cancelDialog();
    formData = new FormData(form);
    const user = formData.get("user");
    const pass = formData.get("pass");


    if (user.trim() === "" || pass.trim() === "") {
        showDialog("Login Form Incomplete", "There are missing fields to create an account", "error");
        return;
    }


    window.connection.login(user, pass);
})

function closeDialog(time) {
    timeout = setTimeout(() => {
        dialog.className = "dialog hidden";
    }, time)
}


window.connection.onLoginStatus((value) => {
    if (value === "null") {
        showDialog("Login Failed", "User or password may be incorrect", "error");
    }
    else if (value !== "pending") {
        console.log(value);
        const user = JSON.parse(value);
        if (remember.checked) {
            const { username, password } = user;
            localStorage.setItem("login-info", JSON.stringify({ username, password }))
        }
        else if (localStorage.getItem("login-info")) {
            localStorage.removeItem("login-info");
        }
        showDialog("Welcome", "Login success, enjoy your game!", "success");

        setTimeout(() => {
            window.location.replace("./game.html")
        }, 3000);
    }
})

window.connection.onSignupStatus((value) => {
    if (value === "success") {
        showDialog("Account Created!", "Welcome to our platform", "success");
        userField.value = "";
        passField.value = "";
    }
    else if (value === "failed") {
        showDialog("Eror while creating an account", "An account with the same username may already exists", "success");
    }
})