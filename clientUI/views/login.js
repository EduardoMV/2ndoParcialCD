const form = document.getElementById("loginForm");
const login = document.getElementById("loginBtn");
const signup = document.getElementById("signupBtn");
const dialog = document.querySelector("div.dialog");
const dialogTitle = document.querySelector("h3.dialog-title");
const dialogInfo = document.querySelector("p.dialog-info");

form.onsubmit = (evt) => evt.preventDefault();

let timeout = null;
let formData = null;

function cancelDialog() {
    if (timeout === null) return;
    clearTimeout(timeout);
    dialog.className = "dialog hidden";
}

signup.addEventListener("click", () => {
    cancelDialog();
    formData = new FormData(form);
    const user = formData.get("user");
    const pass = formData.get("pass");
    window.connection.signup(user, pass);
})

login.addEventListener("click", () => {
    cancelDialog();
    formData = new FormData(form);
    const user = formData.get("user");
    const pass = formData.get("pass");
    window.connection.login(user, pass);
})

function closeDialog(time) {
    timeout = setTimeout(() => {
        dialog.className = "dialog hidden";
    }, time)
}


window.connection.onLoginStatus((value) => {
    console.log(`login ${value}`);
    if (value === "success") {

        dialog.className = "dialog success";
        dialogTitle.innerHTML = "Success :)";
        dialogInfo.innerHTML = "Account authentificated";

        setTimeout(() => {
            window.location.replace("./game.html")
        }, 3000);
        closeDialog(3000);
    }
    else if (value === "failed") {
        dialog.className = "dialog error"
        dialogTitle.innerHTML = "Error"
        dialogInfo.innerHTML = "The user or the password may be incorrect";
        closeDialog(3000);
    }
})

window.connection.onSignupStatus((value) => {
    console.log(`signup ${value}`);
    if (value === "success") {

        dialog.className = "dialog success";
        dialogTitle.innerHTML = "Success :)";
        dialogInfo.innerHTML = "Account created, Now you can LogIn";

        setTimeout(() => {
            window.location.replace("./game.html")
        }, 3000);
        closeDialog(3000);
    }
    else if (value === "failed") {
        dialog.className = "dialog error"
        dialogTitle.innerHTML = "Error"
        dialogInfo.innerHTML = "Couldn't create a new account";
        closeDialog(3000);
    }
})