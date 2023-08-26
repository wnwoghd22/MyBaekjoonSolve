############## 주의 ##############
# 입력을 받기위한 input 함수는 절대 사용하지 않습니다.
# 내장 함수 sum, min, max, len 함수를 사용하지 않습니다.
# 사용시 감점처리 되니 반드시 확인 바랍니다.
def caesar(word, num):
    ans = str()                 # 답을 문자열 형태로 만들어서 반환
    for i in word:
        if 65 <= ord(i) <= 90:  # 만일 대문자라면?
            ans += chr(65 + ((ord(i) - 65 + (num % 26)) % 26))
            # A는 65번째 문자이니 65부터 시작해 밀린 숫자만큼 나머지 연산을 해서 더한다.
            # Z를 넘어가면 한 바퀴를 돌아서 더해야 하니 다시 26만큼 나머지 연산
        if 97 <= ord(i) <= 122:
            ans += chr(97 + ((ord(i) - 97 + (num % 26)) % 26))
            continue
    return ans
    # 여기에 코드를 작성하여 함수를 완성합니다.
    


# 아래 코드는 실행 확인을 위한 코드입니다.
if __name__ == '__main__':
    # 예시 코드는 수정하지 마세요.
    print(caesar('ssafy', 1))   # => ttbgz
    print(caesar('Python', 10)) # => Zidryx
    # 여기부터 아래에 추가 테스트를 위한 코드 작성 가능합니다.
    