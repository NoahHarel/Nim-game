"""
This code includes an interesting bug, can you find it? ;)
Author: Omer Rosenbaum
"""

# Looney Tune structure:
# [name, friends_list, age]
# name - str, friends_list - list, age - int
NAME = 0
FRIENDS_LIST = 1
AGE = 2


def show_looney_friends(looney_tune):
    """this function shows the friends of a given looney tune"""
    if len(looney_tune[FRIENDS_LIST]) == 0:
        print("%s has no friends :(" % (looney_tune[NAME],))

    else:
        print("Those are %s's friends:" % (looney_tune[NAME],))
        for friend in looney_tune[FRIENDS_LIST]:
            print("%s is %d years old" % (friend[NAME], friend[AGE]))

    # Added for output readability
    print('==========')


def add_new_friend(looney_tune, new_friend):
    """this functon adds new_friend to the looney_tune's list of friends"""
    looney_tune[FRIENDS_LIST].append(new_friend)


def create_looney_tune(name="cool guy!", friends_list=None, age=0):
    """Creates a new looney tune and returns it."""
    if friends_list==None:
        friends_list=[]
    return [name, friends_list, age]


def show_friends(looneys_list):
    """shows friends for all looney tunes in lonneys_list (list or tuple)"""
    for looney_tune in looneys_list:
        show_looney_friends(looney_tune)


def main():
    """Main function used to test the code"""
    bugs = create_looney_tune("Bugs Bunny", age=2)
    daffy = create_looney_tune("Daffy Duck")
    melissa = create_looney_tune("Melissa Duck", [daffy], 4)
    yosemite = create_looney_tune("Yosemite Sam", [bugs, melissa], 302)

    all_looney_tunes = (bugs, daffy, melissa, yosemite)
    show_friends(all_looney_tunes)

    print('+++ adding friends +++')

    add_new_friend(daffy, bugs)
    add_new_friend(yosemite, daffy)

    show_friends(all_looney_tunes)


if __name__ == '__main__':
    main()
