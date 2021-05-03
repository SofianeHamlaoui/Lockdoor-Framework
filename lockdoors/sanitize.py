# input sanitization function
def bash_escape_restrictor(data):
    escape_list = [";","&","|","*"," "]
    for charecter in data:
        if charecter in escape_list:
            data = data.split(charecter)[0]
            break
    if data == '':
        print("\n\n[+] please provide a VALID DATA\n\n")
    else :
        print("[+] link : {}".format(data))
    return data