def morse_signal_duration(input_str: str) -> int:
    # Morse code binary representation from A-Z and 0-9
    morsecode = [
        "10111", "111010101", "11101011101", "1110101", "1", "101011101",
        "111011101", "1010101", "101", "1011101110111", "111010111", "101110101",
        "1110111", "11101", "11101110111", "10111011101", "1110111010111",
        "1011101", "10101", "111", "1010111", "101010111", "101110111",
        "11101010111", "1110101110111", "11101110101",  # A-Z (0–25)
        "1110111011101110111", "10111011101110111", "101011101110111",  # 0–2
        "1010101110111", "10101010111", "101010101", "11101010101",      # 3–6
        "1110111010101", "111011101110101", "11101110111011101"          # 7–9
    ]
    
    total_duration = 0

    for word in input_str.upper().split():
        for char in word:
            if char.isalpha():
                index = ord(char) - ord('A')
            elif char.isdigit():
                index = ord(char) - ord('0') + 26
            else:
                continue  # Ignore non-alphanumeric characters

            code = morsecode[index]
            total_duration += len(code) * 100  # Each 0 or 1 is 100ms

        # Add 700ms space after each word except the last
        total_duration += 700

    # Remove trailing 700ms if any word was processed
    if total_duration >= 700:
        total_duration -= 700

    return total_duration

print("Duration: ", morse_signal_duration("I Can Morse"))