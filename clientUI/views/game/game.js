let playerCount = 0, dealerCount = 0, playerAces = 0, dealerAces = 0
let hiddenCard, deck = []
let canHitMe = true
let stayBtn = document.getElementById('stay')
let hitBtn = document.getElementById('hitMe')
let result = document.getElementById('result')
let dealerCountText = document.getElementById('dealer-count')
let playerCountText = document.getElementById('player-count')

buildDeck()
shuffleDeck()
startGame()

hitBtn.addEventListener('click', () => {
  addCardToHand("user")
})
stayBtn.addEventListener('click', () => {
  stay()
})
console.log("Si jale perros!")
console.log(deck)

function startGame() {
  hiddenCard = deck.pop()
  dealerCount += getCardValue(hiddenCard)
  dealerAces += countAces(hiddenCard)
  giveDealerCards()
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
  dealerCount, dealerAces = countAces(dealerCount, dealerAces)

  playerCount, playerAces = countAces(playerCount, playerAces)
  canHitMe = false
  document.getElementById('hidden-card').src = "./cards/" + hiddenCard + ".png"
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
  while (dealerCount < 17) {
    let card = deck.pop()
    console.log(card)
    let cardView = document.createElement("img")
    cardView.src = "./cards/" + card + ".png"
    dealerCount += getCardValue(card)
    dealerAces += isItAnAce(card)
    console.log("card View: ", cardView)
    document.getElementById('dealer-cards').append(cardView)
  }
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
  hand.appendChild(cardImg);
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

