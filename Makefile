CSRCS 		:= 	main.c
CLIBS 		:= 	-lncurses -lm

CAPP 			:= 	pidapp
CDEBUG 		:= 	$(CAPP)-debug

CDEBUGDEF	:= 	-DDEBUG		\
							-DBORDERS

CXX				:= 	gcc

.PHONY: all
all: app debug

.PHONY: app
app: $(CAPP)

$(CAPP): $(CSRCS)
	@echo -e "\nBuilding $(CAPP)..."
	$(CXX) $(CSRCS) -o$(CAPP) $(CLIBS)
	@echo -e "...done!\n"

.PHONY: debug
debug: $(CDEBUG)

$(CDEBUG): $(CSRCS)
	@echo -e "\nBuilding $(CDEBUG)..."
	$(CXX) $(CSRCS) -o$(CDEBUG) $(CLIBS) $(CDEBUGDEF)
	@echo -e "...done!\n"

.PHONY: clean
clean: 
	@echo -e "\nCleaning up..."
	rm -f $(CAPP) $(CDEBUG)
	@echo -e "...done!\n"
