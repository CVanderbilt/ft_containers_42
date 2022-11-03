SRCS			= tester/map_tester_ft.cpp\
					tester/map_tester_std.cpp\
					tester/progressBar.cpp\
					tester/stack_tester_ft.cpp\
					tester/stack_tester_std.cpp\
					tester/tester_main.cpp\
					tester/vector_tester_ft.cpp\
					tester/vector_tester_std.cpp
OBJS			= $(SRCS:.cpp=.o)

CXX				= clang++
RM				= rm -f
CXXFLAGS		= -std=c++98 -Wall -Wextra -Werror -I.

NAME			= ft_containers

MAP_TEST_SAMPLE_SIZE	= 1000000
VECTOR_TEST_SAMPE_SIZE	= 100000
STACK_TEST_SAMPLE_SIZE	= 1000000 

all:			$(NAME)

.cpp.o:
				$(CXX) $(CXXFLAGS) -o $@ -c $<

$(NAME):		$(OBJS)
				$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
				$(RM) $(OBJS)

fclean:			clean
				$(RM) $(NAME) ft.txt std.txt

re:				fclean $(NAME)

test_map:		fclean $(NAME)
				@rm -f  ft.txt std.txt
				@echo "Running tests... (map)"
				@echo "    Sample size: " $(MAP_TEST_SAMPLE_SIZE)
				@./$(NAME) -map $(MAP_TEST_SAMPLE_SIZE)
				@diff -y --suppress-common-lines --text ft.txt std.txt

test_stack:		fclean $(NAME)
				@rm -f  ft.txt std.txt
				@echo "Running tests... (stack)"
				@echo "    Sample size: " $(STACK_TEST_SAMPLE_SIZE)
				@./$(NAME) -stack $(STACK_TEST_SAMPLE_SIZE)
				@diff -y --suppress-common-lines --text ft.txt std.txt

test_vector:	fclean $(NAME)
				@rm -f  ft.txt std.txt
				@echo "Running tests... (vector)"
				@echo "    Sample size: " $(VECTOR_TEST_SAMPLE_SIZE)
				@./$(NAME) -vector $(VECTOR_TEST_SAMPLE_SIZE)
				@diff -y --suppress-common-lines --text ft.txt std.txt

test_map_leaks:		fclean $(NAME)
					@rm -f  ft.txt std.txt
					@echo "Running tests... (map)"
					@echo "    Sample size: " $(MAP_TEST_SAMPLE_SIZE)
					@./$(NAME) -map $(MAP_TEST_SAMPLE_SIZE) s

test_stack_leaks:	fclean $(NAME)
					@rm -f  ft.txt std.txt
					@echo "Running tests... (stack)"
					@echo "    Sample size: " $(STACK_TEST_SAMPLE_SIZE)
					@./$(NAME) -stack $(STACK_TEST_SAMPLE_SIZE) s

test_vector_leaks:	fclean $(NAME)
					@rm -f  ft.txt std.txt
					@echo "Running tests... (vector)"
					@echo "    Sample size: " $(VECTOR_TEST_SAMPLE_SIZE)
					@./$(NAME) -vector $(VECTOR_TEST_SAMPLE_SIZE) s

test_all:
					make test_map
					make test_stack
					make test_vector


.PHONY:	test_map test_stack test_vector test_map_leaks test_stack_leaks test_vector_leaks test_all all clean fclean re