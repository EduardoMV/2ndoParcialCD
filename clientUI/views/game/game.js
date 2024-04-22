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
game()

hitBtn.addEventListener('click', () => {
  hitMe()
})
stayBtn.addEventListener('click', () => {
  stay()
})
console.log("Si jale perros!")
console.log(deck)
console.log(hiddenCard)

function game() {
  hiddenCard = deck.pop()
  dealerCount += getCardValue(hiddenCard)
  dealerAces += checkForAce(hiddenCard)

  while (dealerCount < 17) {
    let card = deck.pop()
    let cardView = document.createElement("img")
    cardView.src = "./card/" + card + ".png"
    dealerCount += getCardValue(card)
    dealerAces += isItAnAce(card)
    document.getElementById('dealer-cards').append(cardView);

    givePlayerCards()

  }
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
  if (typeof (value) == 'number') {
    return parseInt(value)
  } else {
    if (value == 'A') {
      return 11
    } else {
      return 10
    }
  }
}

function stay() {
  dealerCount, acesCount = countAces(dealerCount, acesCount)

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

function givePlayerCards() {
  for (let i = 0; i < 2; i++) {
    let card = deck.pop()
    let cardView = document.createElement("img")
    cardView.src = "./card/" + card + ".png"
    playerCount += getCardValue(card)
    playerAces += isItAnAce(card)
    document.getElementById('player-cards').append(cardView);
  }
}

function hitMe() {
  if (!canHitMe) return
  let card = deck.pop()
  let cardView = document.createElement("img")
  cardView.src = "./card/" + card + ".png"
  playerCount += getCardValue(card)
  playerAces += isItAnAce(card)
  document.getElementById('player-cards').append(cardView);

  playerCount, playerAces = countAces(playerCount, playerAces)

  if (playerCount > 21) {
    canHitMe = false
  }
}

