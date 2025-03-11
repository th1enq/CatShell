package pipeline

import (
	"cat-shell/helper"
	"io"
	"os"
	"os/exec"
	"strings"

	"github.com/mattn/go-shellwords"
)

func ExecutePipeline(command string) error {
	commandSplit := strings.Split(command, "|")

	var prevCmd *exec.Cmd
	var prevPipe io.ReadCloser

	for i, value := range commandSplit {
		args, err := shellwords.Parse(strings.TrimSpace(value))

		if err != nil {
			return helper.ErrSomethingWentWrong
		}

		if len(args) == 0 {
			continue
		}

		cmd := exec.Command(args[0], args[1:]...)

		if prevPipe != nil {
			cmd.Stdin = prevPipe
		}

		if i < len(commandSplit)-1 {
			pipe, err := cmd.StdoutPipe()

			if err != nil {
				return helper.ErrSomethingWentWrong
			}
			prevPipe = pipe
		} else {
			cmd.Stdout = os.Stdout
		}

		if err := cmd.Start(); err != nil {
			return helper.ErrSomethingWentWrong
		}
		if prevCmd != nil {
			prevCmd.Wait()
		}
		prevCmd = cmd
	}

	if prevCmd != nil {
		prevCmd.Wait()
	}

	return nil
}
