# Wykryj OS i ustaw odpowiednio komendy
ifeq ($(OS),Windows_NT)
	OUTPUT_CMD = simulation.exe
	CREATE_DIR = @if not exist $(OBJDIR) mkdir $(OBJDIR)
	CLEAN_CMD = del /q $(OBJDIR)\* $(OUTPUT) *.ilk *.pdb
	REMOVE_DIR = @if exist $(OBJDIR) rmdir /S /Q $(OBJDIR)
else
	OUTPUT_CMD = simulation
	CREATE_DIR = mkdir -p $(OBJDIR)
	CLEAN_CMD = rm -rf $(OBJDIR) $(OUTPUT) *.ilk *.pdb
	REMOVE_DIR = rm -rf $(OBJDIR)
endif

# Ustaw compilator pod clang
CC = clang

# Folder dla plików typu header
INCLUDEDIR = include
# Folder dla plików źródłowych
SRCDIR = src
# Folder dla plików obiektowych
OBJDIR = build

# Flags
CFLAGS = -g -Wall -I$(INCLUDEDIR)

# Nazwa pliku binarnego
OUTPUT = $(OUTPUT_CMD)

# Lista plików źródłowych
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/buildings.c $(SRCDIR)/graveyard.c $(SRCDIR)/menu.c $(SRCDIR)/town.c $(SRCDIR)/resident.c $(SRCDIR)/death.c $(SRCDIR)/simulation.c $(SRCDIR)/save_load.c

# Lista plików obiektowych
OBJECTS = $(OBJDIR)/main.o $(OBJDIR)/buildings.o $(OBJDIR)/graveyard.o $(OBJDIR)/menu.o $(OBJDIR)/town.o $(OBJDIR)/resident.o $(OBJDIR)/death.o $(OBJDIR)/simulation.o $(OBJDIR)/save_load.o

# Podstawowe zasady
all: $(OUTPUT)

# Stwórz finalny plik wykonywalny
$(OUTPUT): $(OBJECTS)
# $@ oznacza nazwę celu (OUTPUT), a $^ to wszystkie pliki zależne (OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Stworzenie plików obiektowych .o z plików źródłowych .c
$(OBJDIR)/%.o: $(SRCDIR)/%.c
# Stwórz folder na pliki źródłowe
	$(CREATE_DIR)
# Kompilacja pliku źródłowego ($<) do pliku obiektowego ($@)
	$(CC) $(CFLAGS) -c $< -o $@

# Usuń stworzone pliki
clean:
	$(CLEAN_CMD)
	$(REMOVE_DIR)