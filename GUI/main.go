package main

import (
	"image/color"

	"fyne.io/fyne/v2"
	"fyne.io/fyne/v2/app"
	"fyne.io/fyne/v2/canvas"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/dialog"
	"fyne.io/fyne/v2/layout"
	"fyne.io/fyne/v2/widget"
)

type User struct {
	Name     string `json:"name"`
	Password string `json:"password"`
	Data     []byte `json:"-"`
}

func newUser(name string, password string) *User {
	user := &User{}
	user.Name = name
	user.Password = password
	user.Data = []byte("{\n\tName: " + name + ", \n\tPassword: " + password + "\n}\n")
	return user
}

func signInLayout(username, password *widget.Entry, button *widget.Button) *fyne.Container {
	return container.NewGridWithRows(3,
		widget.NewLabel("Welcome to the Whiteboard! Please Log In"),
		container.NewGridWithColumns(3,
			layout.NewSpacer(),
			container.NewVBox(
				username,
				password,
				button,
			),
			layout.NewSpacer(),
		),
		layout.NewSpacer(),
	)
}

func whiteBoard(teacher, class, college *widget.Label) *fyne.Container {
	return container.NewBorder(
		widget.NewLabel("Welcome to our Whiteboard!"),
		container.NewGridWithColumns(3,
			teacher,
			class,
			college,
		),
		layout.NewSpacer(),
		layout.NewSpacer(),
		canvas.NewRectangle(color.White),
	)
}

func aboutApp() *fyne.Container {
	return container.NewBorder(
		widget.NewLabel("About our app"),
		widget.NewLabel("Todos los derechos reservados a los contribuidores de la APP"),
		layout.NewSpacer(),
		layout.NewSpacer(),
	)
}

func main() {
	a := app.New()
	w := a.NewWindow("Whiteboard")
	w.Resize(fyne.NewSize(550, 425))

	user := &widget.Entry{PlaceHolder: "Username"}
	password := &widget.Entry{PlaceHolder: "Password", Password: true}
	teacher := widget.NewLabel("Maestro: Dr. Juan Carlos Pimentel")
	class := widget.NewLabel("Computo Distribuido")
	college := widget.NewLabel("Universidad Panamericana")
	login := widget.NewButton("Login", func() {
		if user.Text == "" && password.Text == "" {
			dialog.ShowInformation("ERROR!", "You must fill all spaces in order to have access to the whiteboard.", w)
		} else if user.Text == "" {
			dialog.ShowInformation("ERROR!", "You must have a username!", w)
		} else if password.Text == "" {
			dialog.ShowInformation("ERROR!", "You must have a password!", w)
		} else {
			u := newUser(user.Text, password.Text)
			addUserToDB(u.Data)
			dialog.ShowInformation("Awesome!", "Happy drawing "+user.Text+"!", w)
			w.SetContent(whiteBoard(teacher, class, college))
		}
	})
	w.SetContent(signInLayout(user, password, login))

	w.ShowAndRun()
}
