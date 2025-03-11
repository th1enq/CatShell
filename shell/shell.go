package shell

import (
	"bufio"
	"cat-shell/executor"
	"fmt"
	"os"
	"strings"
)

func Start() {
	reader := bufio.NewReader(os.Stdin)

	for {
		fmt.Print("$cat-shell> ")

		// read the command
		command, _ := reader.ReadString('\n')
		command = strings.TrimSpace(command)

		// exit
		if command == "exit" {
			break
		}

		// Execute the command
		executor.Execute(command)
	}
}
