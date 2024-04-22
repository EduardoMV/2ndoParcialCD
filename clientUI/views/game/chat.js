const chatField = document.getElementById("chatField");
const chatList = document.getElementById("chatList");

let username;

window.onload = async () => {
    const user = JSON.parse(await window.userData.getUserData());
    username = user.username;
    window.game.connect();

}

chatField.addEventListener("keypress", (evt) => {
    if (evt.key == "Enter") {
        evt.preventDefault();

        const fieldFormat = new RegExp("^[a-zA-Z0-9_ ()¿?¡!#$%/]*$");
        const value = chatField.value;

        console.log(value);

        if (fieldFormat.test(value) && value.trim().length > 0) {
            const [cmd, tag, ...msg] = value.split(" ");
            let to = cmd === "/to" && tag !== undefined ? tag : "everyone";
            console.log(cmd, tag, msg);
            window.game.send("chat", username, to, to === "everyone" ? value : msg.join(" "));
            chatField.value = "";
        }
    }
})

function createBubble(user, msg) {
    const msgDiv = document.createElement("div");
    msgDiv.classList.add("chat-msg");

    const bubble = document.createElement("div");
    bubble.classList.add("chat-bubble");
    bubble.classList.add(user === username ? "me" : "other");

    const bubbleUser = document.createElement("span");
    const userText = document.createTextNode(user);
    bubbleUser.appendChild(userText);

    const bubbleMsg = document.createElement("p");
    const msgText = document.createTextNode(msg);
    bubbleMsg.appendChild(msgText);

    bubble.appendChild(bubbleUser);
    bubble.appendChild(bubbleMsg);

    msgDiv.appendChild(bubble);

    chatList.appendChild(msgDiv);

}

function createServerInfo(msg) {
    const msgDiv = document.createElement("div");
    msgDiv.classList.add("server-msg");
    const msgText = document.createTextNode(msg);
    msgDiv.appendChild(msgText);

    chatList.appendChild(msgDiv);
}

window.game.onMessage((val) => {
    const [cmd, value] = val.split(":");
    if (cmd !== "chat") return;

    const [userProp, toProp, msgProp] = value.split("&");
    const [_user, user] = userProp.split("=");
    const [_to, to] = toProp.split("=");
    let [_msg, msg] = msgProp.split("=");

    if (to === "everyone" || to === username || user === username) {
        msg = msg.replace(/['"]+/g, '')
        if (user === "server") createServerInfo(msg);
        else createBubble(user, msg);

    }

})