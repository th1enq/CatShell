package executor

import (
	"cat-shell/helper"
	"cat-shell/pipeline"
	"cat-shell/redirection"
	"fmt"
	"os"
	"os/exec"
	"strings"

	"github.com/mattn/go-shellwords"
)

func Execute(originCommands string) {
	listOfCommands := strings.Split(originCommands, "&&")

	for _, command := range listOfCommands {
		// Erase the first space in each single command
		command = strings.TrimSpace(command)

		if strings.Contains(command, "&") {
			cmd := strings.Split(command, "&")

			for _, value := range cmd {
				handleCommand(value, helper.WAING)
			}

			continue
		}
		handleCommand(command, helper.NO_WAITING)
	}
}

func handleCommand(command string, waitingFlag bool) {
	// Handling pipeline
	if strings.Contains(command, "|") {
		pipeline.ExecutePipeline(command)
		return
	}

	// Handing >> redirection: Append output
	// Handing >  redirection: Overwrite output
	if strings.Contains(command, ">") {
		handleError(redirection.ExecuteRedirectionOutput(command, waitingFlag))
		return
	}

	// Handing < redirection: Input
	if strings.Contains(command, "<") {
		handleError(redirection.ExecuteRedirectionInput(command, waitingFlag))
		return
	}

	handleError(ExecuteSimpleCommand(command, waitingFlag))
}

func handleError(err error) {
	if err != nil {
		fmt.Println(err)
	}
}

func ExecuteSimpleCommand(command string, waitingFlag bool) error {
	// Split into array
	args, err := shellwords.Parse(command)

	if err != nil {
		return helper.ErrSomethingWentWrong
	}

	if len(args) == 0 {
		return nil
	}

	// Set the command
	cmd := exec.Command(args[0], args[1:]...)

	// Get the output and error
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr

	// Handle errors
	if !waitingFlag {
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
