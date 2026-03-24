#!/bin/bash

echo "Testing basic commands..."

# Test 1: Clean and exit
echo "Test 1: clean and exit"
echo -e "clean\nexit" | ./code

echo -e "\nTest 2: Add first user"
echo -e "add_user -c admin -u user1 -p pass123 -n 张三 -m test@test.com -g 5\nclean\nexit" | ./code

echo -e "\nTest 3: Add user with proper flow"
echo -e "add_user -c admin -u admin -p admin123 -n Admin -m admin@test.com -g 10\nlogin -u admin -p admin123\nadd_user -c admin -u user2 -p pass456 -n 李四 -m user2@test.com -g 5\nquery_profile -c admin -u user2\nlogout -u admin\nexit" | ./code