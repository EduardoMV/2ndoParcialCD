const chatField = document.getElementById("chatField");
const chatList = document.getElementById("chatList");
const chatToggler = document.getElementById("toggle-chat");
const chatDiv = document.getElementById("chat")

chatField.addEventListener("keypress", (evt) => {
    if (evt.key == "Enter") {
        evt.preventDefault();

        const fieldFormat = new RegExp("^[a-zA-Z0-9_ ()¿?¡!#$%/]*$");
        const value = chatField.value;

        if (fieldFormat.test(value) && value.trim().length > 0) {
            const [cmd, tag, ...msg] = value.split(" ");
            let to = cmd === "/to" && tag !== undefined ? tag : "everyone";
            window.game.send(username, to, to === "everyone" ? value : msg.join(" "));
            chatField.value = "";
        }
    }
})

const removeUnreadMsg = undefined;
chatToggler.addEventListener("click", () => {
    chatDiv.classList.toggle("hidden");
    if (!chatDiv.classList.contains("hidden")) {
        chatToggler.classList.remove("unread");
        if (removeUnreadMsg === undefined) {
            const serverMsg = document.getElementById("unreadServerMsg");
            if (!serverMsg) return;
            removeUnreadMsg = setTimeout(() => {
                serverMsg.remove();
            }, 1500);
        }
    }
})


function createBubble(user, msg) {
    if (chatDiv.classList.contains("hidden")) {
        window.connection.sendNotification(`${user} sent you a message`, msg);
        if (!chatDiv.classList.contains("unread")) {
            chatToggler.classList.add("unread");

            if (!document.getElementById("unreadServerMsg")) {
                const unreadMsg = createServerInfo("new messages");
                unreadMsg.id = "unreadServerMsg";
            }
        }
    }

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

    chatList.scrollTop = chatList.scrollHeight;

    return msgDiv;
}

function createServerInfo(msg) {
    const msgDiv = document.createElement("div");
    msgDiv.classList.add("server-msg");
    const msgText = document.createTextNode(msg);
    msgDiv.appendChild(msgText);

    chatList.appendChild(msgDiv);

    return msgDiv;
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