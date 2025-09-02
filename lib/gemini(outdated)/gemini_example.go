package main

import (
	"context"
	"fmt"
	"log"
	"os"

	"google.golang.org/genai"
)

func GeminiExample() {
	ctx := context.Background()
	// GeminiExample() //go run lib/gemini/gemini_example.go lib/gemini/gemini.go
	apiKey := os.Getenv("GEMINI_API_KEY")
	if apiKey == "" {
		log.Fatal("GEMINI_API_KEY environment variable not set")
	}

	client, err := genai.NewClient(ctx, &genai.ClientConfig{
		APIKey:  apiKey,
		Backend: genai.BackendGeminiAPI,
	})

	if err != nil {
		log.Fatal(err)
	}

	result, err := client.Models.GenerateContent(
		ctx,
		"gemini-2.0-flash",
		genai.Text("Explain how AI works in a few words"),
		nil,
	)

	if err != nil {
		log.Fatal(err)
	}

	fmt.Println(result.Text())
}
