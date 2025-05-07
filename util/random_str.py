# エニグマのローター生成用

import random
ascii_chars = [chr(i) for i in range(33, 127)]

random.shuffle(ascii_chars)

randomized_str = ''.join(ascii_chars)
print(randomized_str)
