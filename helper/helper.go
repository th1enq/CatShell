package helper

import "errors"

var (
	ErrSomethingWentWrong = errors.New("something went wrong, try again")
	ErrorFileOpening      = errors.New("error opening file, try again")
)

const (
	NO_WAITING = false
	WAING      = true
)
