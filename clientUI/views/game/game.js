let stayBtn = document.getElementById('stay')
let hitBtn = document.getElementById('hitMe')
let startBtn = document.getElementById('start')

//NEW VARS
const hiddenCardElement = document.querySelector('#hidden-card');
const playerHandsContainerElement = document.querySelector('#player-hands');

let playerCount = 0;
let currentTurn = 0;
let players = [];
let cardCant = 0;
let cardCantDealer = 0;
let hiddenCard = null;

let username;

/* Loads the username */
(async () => {
    const user = JSON.parse(await window.userData.getUserData());
    username = user.username;
    window.game.connect();
    window.game.cmd(username, "join", "null");
})();

stayBtn.addEventListener('click', () => {
    window.game.cmd(username, "turn");
})

hitBtn.addEventListener('click', () => {
    window.game.cmd(username, "take");
})

startBtn.addEventListener('click', () => {
    window.game.cmd(username, "start");
})

function moveToNextPlayer(pos = currentTurn) {
    if (pos >= players.length) {
        playerHandsContainerElement.style.transform = `translate(-${0 * pos}vw)`;

    }
    else playerHandsContainerElement.style.transform = `translate(-${100 * pos}vw)`;
}

/*
hitBtn.addEventListener('click', () => {
    addCardToHand("user")
})
buildDeck()
shuffleDeck()
startGame()
console.log(deck)

function startGame() {
  hiddenCard = deck.pop()
  dealerCount += getCardValue(hiddenCard)
  dealerAces += countAces(hiddenCard)
  const cardImg = hiddenCardElement.getElementsByClassName("card-front")[0];
  console.log(`hiddenCard: ${hiddenCard}`)
  cardImg.src = "./cards/" + hiddenCard + ".png"

  hiddenCardElement.className = "card hidden";
  const card = deck.pop()
  document.getElementById('dealer-cards').appendChild(createCardElement(card))
  //giveDealerCards()
  // givePlayerCards("user")
}
function buildDeck() {
  let cards = ['2', '3', '4', '5', '6', '7', '8', '9', '10', 'J', 'Q', 'K', 'A']
  let values = ['C', 'H', 'D', 'S']
  for (i = 0; i < values.length; i++) {
    for (j = 0; j < cards.length; j++) {
      deck.push(cards[j] + '-' + values[i])
    }
  }
}

function shuffleDeck() {
  for (let i = 0; i < deck.length; i++) {
    let r = Math.floor(Math.random() * deck.length)
    let aux = deck[r]
    deck[i] = deck[r]
    deck[r] = aux
  }
}

function getCardValue(card) {
  let data = card.split('-') // ["10", "H"]
  let value = data[0]
  if (isNaN(value)) {
    if (value == 'A') {
      return 11
    }
    return 10
  }
  return parseInt(value)
}

function stay() {
  canHitMe = false;
  hiddenCardElement.classList.toggle("hidden");
  giveDealerCards();

  dealerCount, dealerAces = countAces(dealerCount, dealerAces)
  playerCount, playerAces = countAces(playerCount, playerAces)

  let message = ""
  if (playerCount > 21 || (playerCount < dealerCount && dealerCount <= 21)) {
    message = "You have lost!"
  }
  else if (dealerCount > 21 || (playerCount > dealerCount && playerCount <= 21)) {
    message = "You have won!"
  } else {
    message = "It's a tie!"
  }

  playerCountText.innerText = playerCount
  dealerCountText.innerText = dealerCount
  result.innerText = message
}

function isItAnAce(card) {
  let data = card.split('-')
  if (data[0] == 'A') {
    return 1
  }
  return 0
}

function countAces(totalCount, acesCount) {
  while (acesCount > 0 && totalCount > 21) {
    totalCount -= 10
    acesCount -= 1
  }
  return totalCount, acesCount
}
function giveDealerCards() {
  const interval = setInterval(() => {
    if (dealerCount >= 17) {
      clearInterval(interval);
      return;
    }

    let card = deck.pop()
    console.log(card)
    let cardView = document.createElement("img")
    cardView.src = "./cards/" + card + ".png"
    dealerCount += getCardValue(card)
    dealerAces += isItAnAce(card)
    console.log("card View: ", cardView)
    document.getElementById('dealer-cards').appendChild(createCardElement(card))

  }, 1000);
}

function givePlayerCards(user) {
  for (let i = 0; i < 2; i++) {
    let card = deck.pop()
    let cardView = document.createElement("img")
    cardView.src = "./cards/" + card + ".png"
    playerCount += getCardValue(card)
    playerAces += isItAnAce(card)
    document.getElementById(`playerHand-${user}`).append(cardView);
  }
}

// function hitMe(user) {
//   if (!canHitMe) return
//   let card = deck.pop()
//   let cardView = document.createElement("img")
//   cardView.src = "./cards/" + card + ".png"
//   playerCount += getCardValue(card)
//   playerAces += isItAnAce(card)
//   document.getElementById(`playerHand-${user}`).append(cardView);

//   playerCount, playerAces = countAces(playerCount, playerAces)

//   if (playerCount > 21) {
//     canHitMe = false
//     console.log("You can no loger hit!")
//   }
// }

function addCardToHand(user) {
  if (!canHitMe) return
  const hand = document.getElementById(`playerHand-${user}`);
  const card = deck.pop()
  playerCount += getCardValue(card)
  playerAces += isItAnAce(card)
  const cardImg = document.createElement("img");
  cardImg.src = `./cards/${card}.png`
  cardImg.className = "card";
  hand.appendChild(createCardElement(card));
  const classes = ["one", "two", "three", "four", "five"];

  hand.classList.remove("one");
  hand.classList.remove("two");
  hand.classList.remove("three");
  hand.classList.remove("four");
  hand.classList.remove("five");

  hand.classList.add(classes[hand.childNodes.length - 2]);
  console.log(":");

  playerCount, playerAces = countAces(playerCount, playerAces)

  if (playerCount > 21) {
    canHitMe = false
    console.log("You can no loger hit!")
  }
}

let counter = 0;

const inte = setInterval(() => {
  if (counter == 2) {
    clearInterval(inte);
    return;
  }
  counter++;
  console.log("new card")
  addCardToHand("user");
}, 1000);
*/

/**
 * Adds a new player element into the players Hands
 * @param {string} user name of the user to be created a HTML element
 * @returns HTMLElement
 */
function createPlayerElement(user) {
    const playerElement = document.createElement("div");
    playerElement.id = `player-${user}`;
    playerElement.className = 'player';

    const playerTitleElement = document.createElement("h2");
    playerTitleElement.className = 'player-title'
    playerTitleElement.innerHTML = `${user} ${user === username ? "(me)" : ""} <span class="player-count"></span>`;

    const playerHandElement = document.createElement("div");
    playerHandElement.className = "player-hand";

    playerElement.appendChild(playerTitleElement);
    playerElement.appendChild(playerHandElement);

    return playerElement;
}

/**
 * Adds a player to the game
 * @param {string} user name of the user to be added
 */
function addPlayer(user) {
    //Avoid creating duplicate players
    if (document.getElementById(`player-${user}`)) return false;

    playerCount++;
    playerHandsContainerElement.style.width = `${players.length * 100}vw`;
    playerHandsContainerElement.appendChild(createPlayerElement(user));

    return true;
}

/**
 * Removes a player from the game
 * @param {string} user name of the user to be removed
 * @returns 
 */
function removePlayer(user) {
    const playerElement = document.getElementById(`player-${user}`);

    //check if the player element exits;
    if (!playerElement) return false;

    playerCount--;
    playerElement.remove()

    return true;
}

/**
 * Creates and HTMLElement for the card 
 * @param {string} card card code to be created
 * @returns 
 */
function createCardElement(card, hidden = false) {
    const cardDiv = document.createElement("div");
    cardDiv.className = `card ${hidden ? "hidden" : ""}`;

    const cardInner = document.createElement("div");
    cardInner.className = "card-inner";

    const cardFront = document.createElement("img");
    cardFront.className = "card-front";
    cardFront.src = `./cards/${card}.png`;

    const cardBack = document.createElement("img");
    cardBack.className = "card-back";
    cardBack.src = './cards/BACK.png';

    cardInner.appendChild(cardFront);
    cardInner.appendChild(cardBack);
    cardDiv.appendChild(cardInner);

    if (hidden) hiddenCard = cardDiv;


    return cardDiv;
}

/**
 * Adds a card to the player hand
 * @param {string} user name of the user to add the card
 * @param {string} card card code to be added
 */
function addCardToPlayer(user, card, hidden = false) {
    const playerHandElement = document.querySelector(`div#player-${user} > div.player-hand`);
    if (!playerHandElement) return false;
    console.log("found")

    playerHandElement.appendChild(createCardElement(card, hidden));

    const classes = ["one", "two", "three", "four", "five"];

    playerHandElement.classList.remove("one");
    playerHandElement.classList.remove("two");
    playerHandElement.classList.remove("three");
    playerHandElement.classList.remove("four");
    playerHandElement.classList.remove("five");

    playerHandElement.classList.add(classes[playerHandElement.childNodes.length - 1]);

    console.log(playerHandElement);
    return true;
}


window.game.onCommand((value) => {
    const [cmd, args] = value.split(":");
    console.log(value);

    if (cmd !== "game") return;

    const [actionProps, dataProps] = args.split("&");
    const [_actionKey, action] = actionProps.split("=");
    let [_dataKey, data] = dataProps.split("=");

    if (action === "join") {
        players = data.split(",");
        playerHandsContainerElement.innerHTML = "";
        playerCount = 0;
        players.forEach(player => {
            addPlayer(player);
        });
    }

    else if (action === "start") {
        if (username === players[0]) {
            startBtn.disabled = true;
            window.game.cmd(username, "take");
            setTimeout(() => {
                window.game.cmd(username, "take");
                setTimeout(() => {
                    window.game.cmd(username, "turn");
                }, 1000);

            }, 500);
        }
    }
    else if (action === "take") {
        data = data.replace('X', '10');
        console.log(`adding to ${currentTurn}`);
        if (currentTurn < players.length) {
            addCardToPlayer(players[currentTurn], data);
            cardCant++;
        } else {

            addCardToPlayer("dealer", data, cardCantDealer === 0);
            cardCantDealer++;
        }
    }
    else if (action === "turn") {
        currentTurn++;
        if (currentTurn > players.length) currentTurn = 0;
        startBtn.disabled = true;
        hitBtn.disabled = true;
        if (currentTurn < players.length) {
            moveToNextPlayer();
            if (username === players[currentTurn]) {
                startBtn.disabled = false;
                hitBtn.disabled = false;
                if (cardCantDealer === 0) {
                    window.game.cmd(username, "take");
                    setTimeout(() => {
                        window.game.cmd(username, "take");
                        setTimeout(() => {
                            window.game.cmd(username, "turn");
                        }, 1000);

                    }, 500);
                }
            }
        }
        else if (currentTurn === players.length) {
            if (username === players[players.length - 1]) {
                if (cardCantDealer === 0) {
                    console.log("dealing...")
                    window.game.cmd("dealer", "take");
                    setTimeout(() => {
                        window.game.cmd("dealer", "take");
                        setTimeout(() => {
                            window.game.cmd("dealer", "turn");
                        }, 1000);

                    }, 500);
                } else {
                    let counter = 0;
                    let interval = setInterval(() => {
                        if (counter > 1) {//dealer points < 17
                            clearInterval(interval);
                            setTimeout(() => {
                                window.game.cmd("dealer", "reset");
                            }, 1000);
                            return;
                        }
                        counter++;
                        window.game.cmd("dealer", "take");
                    }, 1000)

                }
            }
            if (cardCantDealer !== 0) {
                moveToNextPlayer(players.indexOf(username))
                document.querySelector("div#player-dealer > div.player-hand > div.card.hidden").classList.remove("hidden");
            }
        }
    }
    else if (action === "reset") {
        startBtn.disabled = false;
        removePlayer("dealer");
        players.forEach(p => removePlayer(p));
        playerCount = 0;
        currentTurn = 0;
        players = [];
        cardCant = 0;
        cardCantDealer = 0;
        moveToNextPlayer();

        document.getElementById("dealer-cards").appendChild(createPlayerElement("dealer"));

        window.game.cmd(username, "join", "null");
    }


})