package redirection

import (
	"cat-shell/helper"
	"os"
	"os/exec"
	"strings"

	"github.com/mattn/go-shellwords"
)

func ExecuteRedirectionOutput(command string, waitingFlag bool) error {
	/*
		>>: append
		> : overwrite
	*/
	appendFlag := false
	if strings.Contains(command, ">>") {
		appendFlag = true
	}

	// Split into main command and fileName
	commandSplit := strings.Split(command, ">")
	if appendFlag {
		commandSplit = strings.Split(command, ">>")
	}

	/*
		commandSplit[0]: mainCommand
		commandSplit[1]: fileName
	*/
	mainCommand := commandSplit[0]
	fileName := strings.TrimSpace(commandSplit[1])
	if mainCommand == "" {
		return nil
	}
	if fileName == "" {
		return helper.ErrSomethingWentWrong
	}

	//Split the main command
	args, err := shellwords.Parse(mainCommand)
	if len(args) == 0 {
		return nil
	}

	if err != nil {
		return helper.ErrSomethingWentWrong
	}

	var file *os.File

	if appendFlag {
		// Open file
		file, err = os.OpenFile(fileName, os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
	} else {
		// Create file
		file, err = os.Create(fileName)
	}
	if err != nil {
		return err
	}
	defer file.Close()

	// Set the command
	cmd := exec.Command(args[0], args[1:]...)

	// Get the output and error
	cmd.Stdout = file
	cmd.Stderr = os.Stderr

	if waitingFlag == helper.NO_WAITING {
		if err := cmd.Run(); err != nil {
			return helper.ErrSomethingWentWrong
		}
	} else {
		if err := cmd.Start(); err != nil {
			return helper.ErrSomethingWentWrong
		}
	}
	return nil
}

func ExecuteRedirectionInput(command string, waitingFlag bool) error {
	commandSplit := strings.Split(command, "<")

	mainCommand := commandSplit[0]
	fileName := strings.TrimSpace(commandSplit[1])

	file, err := os.Open(fileName)

	if err != nil {
		return helper.ErrorFileOpening
	}
	defer file.Close()

	args, err := shellwords.Parse(mainCommand)

	if err != nil {
		return helper.ErrSomethingWentWrong
	}

	cmd := exec.Command(args[0], args[1:]...)

	cmd.Stdin = file
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr

	if waitingFlag == helper.NO_WAITING {
		if err := cmd.Run(); err != nil {
			return helper.ErrSomethingWentWrong
		}
	} else {
		if err := cmd.Start(); err != nil {
			return helper.ErrSomethingWentWrong
		}
	}

	return nil
}
