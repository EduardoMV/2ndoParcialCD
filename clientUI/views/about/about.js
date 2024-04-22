const startGame = document.getElementById("startGame");
const usernameTxt = document.getElementById("username");
const creditsTxt = document.getElementById("userCredits");

window.onload = async () => {
    const user = JSON.parse(await window.userData.getUserData());
    usernameTxt.innerHTML = user.username;
    creditsTxt.innerHTML = user.credits;
}

startGame.addEventListener("click", () => {
    window.location.replace("./game.html")
})