#include <iniConfig.hpp>
#include <gtest/gtest.h>
#include <fstream>

// Test fixture for Config tests
class ConfigTest : public ::testing::Test {
protected:
    Config config;
    std::string err;
};

// Test loading a valid INI file
TEST_F(ConfigTest, LoadValidFile) {
    ASSERT_TRUE(config.loadFromFile("test_valid.ini", err)) << "Error: " << err;
}

// Test loading a non-existent file
TEST_F(ConfigTest, LoadNonExistentFile) {
    ASSERT_FALSE(config.loadFromFile("nonexistent.ini", err));
    EXPECT_FALSE(err.empty()) << "Error message should not be empty";
    EXPECT_NE(err.find("Could not open"), std::string::npos) 
        << "Error message should mention file open failure";
}

// Test retrieving top-level keys (no section)
TEST_F(ConfigTest, GetTopLevelKeys) {
    ASSERT_TRUE(config.loadFromFile("test_valid.ini", err));
    
    EXPECT_EQ(config.get("", "key1"), "value1");
    EXPECT_EQ(config.get("", "key2"), "value with spaces");
}

// Test retrieving keys from sections
TEST_F(ConfigTest, GetSectionKeys) {
    ASSERT_TRUE(config.loadFromFile("test_valid.ini", err));
    
    EXPECT_EQ(config.get("section1", "host"), "localhost");
    EXPECT_EQ(config.get("section1", "port"), "8080");
    EXPECT_EQ(config.get("section1", "enabled"), "true");
    
    EXPECT_EQ(config.get("section2", "name"), "test database");
    EXPECT_EQ(config.get("section2", "user"), "admin");
    EXPECT_EQ(config.get("section2", "password"), "secret123");
}

// Test default values for missing keys
TEST_F(ConfigTest, DefaultValues) {
    ASSERT_TRUE(config.loadFromFile("test_valid.ini", err));
    
    // Missing key in existing section
    EXPECT_EQ(config.get("section1", "nonexistent", "default"), "default");
    
    // Missing section
    EXPECT_EQ(config.get("nonexistent", "key", "default"), "default");
    
    // Empty default value
    EXPECT_EQ(config.get("nonexistent", "key"), "");
}

// Test comment handling
TEST_F(ConfigTest, CommentHandling) {
    ASSERT_TRUE(config.loadFromFile("test_comments.ini", err));
    
    // Inline comments should be stripped
    EXPECT_EQ(config.get("database", "host"), "localhost");
    EXPECT_EQ(config.get("database", "port"), "3306");
    
    EXPECT_EQ(config.get("server", "address"), "192.168.1.1");
    EXPECT_EQ(config.get("server", "timeout"), "30");
}

// Test whitespace trimming
TEST_F(ConfigTest, WhitespaceTrimming) {
    ASSERT_TRUE(config.loadFromFile("test_whitespace.ini", err));
    
    // Top-level key with surrounding whitespace
    EXPECT_EQ(config.get("", "key1"), "value1");
    
    // Section names should be trimmed
    EXPECT_EQ(config.get("section1", "key2"), "value2");
    EXPECT_EQ(config.get("section1", "key3"), "value3");
    
    EXPECT_EQ(config.get("section2", "key4"), "value with    spaces");
}

// Test malformed line handling
TEST_F(ConfigTest, MalformedLines) {
    ASSERT_TRUE(config.loadFromFile("test_malformed.ini", err));
    
    // Valid lines should still be parsed
    EXPECT_EQ(config.get("", "valid_key"), "valid_value");
    EXPECT_EQ(config.get("section1", "good_key"), "good_value");
    EXPECT_EQ(config.get("section1", "another_good_key"), "another_good_value");
    
    // Malformed lines should be ignored (not present in data)
    EXPECT_EQ(config.get("", "this line has no equals sign"), "");
    EXPECT_EQ(config.get("", "another bad line"), "");
}

// Test empty file
TEST_F(ConfigTest, EmptyFile) {
    // Create empty file
    std::ofstream ofs("test_empty.ini");
    ofs.close();
    
    ASSERT_TRUE(config.loadFromFile("test_empty.ini", err));
    
    // Should return default for any key
    EXPECT_EQ(config.get("", "key", "default"), "default");
    EXPECT_EQ(config.get("section", "key", "default"), "default");
}

// Test file with only comments
TEST_F(ConfigTest, OnlyComments) {
    // Create file with only comments
    std::ofstream ofs("test_only_comments.ini");
    ofs << "; Comment line 1\n";
    ofs << "# Comment line 2\n";
    ofs << "\n";
    ofs << "; Another comment\n";
    ofs.close();
    
    ASSERT_TRUE(config.loadFromFile("test_only_comments.ini", err));
    
    // Should return default for any key
    EXPECT_EQ(config.get("", "key", "default"), "default");
}

// Test reloading (should clear previous data)
TEST_F(ConfigTest, Reloading) {
    ASSERT_TRUE(config.loadFromFile("test_valid.ini", err));
    EXPECT_EQ(config.get("section1", "host"), "localhost");
    
    // Load a different file
    ASSERT_TRUE(config.loadFromFile("test_comments.ini", err));
    
    // Old data should be gone
    EXPECT_EQ(config.get("section1", "host"), "");
    
    // New data should be present
    EXPECT_EQ(config.get("database", "host"), "localhost");
}

// Test empty section name
TEST_F(ConfigTest, EmptySection) {
    // Create file with empty section
    std::ofstream ofs("test_empty_section.ini");
    ofs << "[]\n";
    ofs << "key=value\n";
    ofs.close();
    
    ASSERT_TRUE(config.loadFromFile("test_empty_section.ini", err));
    
    // Empty section should work
    EXPECT_EQ(config.get("", "key"), "value");
}

// Test special characters in values
TEST_F(ConfigTest, SpecialCharacters) {
    // Create file with special characters
    std::ofstream ofs("test_special_chars.ini");
    ofs << "path=/usr/local/bin\n";
    ofs << "url=https://example.com:8080/path?query=value\n";
    ofs << "expression=a+b=c\n";
    ofs.close();
    
    ASSERT_TRUE(config.loadFromFile("test_special_chars.ini", err));
    
    EXPECT_EQ(config.get("", "path"), "/usr/local/bin");
    EXPECT_EQ(config.get("", "url"), "https://example.com:8080/path?query=value");
    EXPECT_EQ(config.get("", "expression"), "a+b=c");
}

// Test empty values
TEST_F(ConfigTest, EmptyValues) {
    // Create file with empty values
    std::ofstream ofs("test_empty_values.ini");
    ofs << "key1=\n";
    ofs << "key2=  \n";
    ofs.close();
    
    ASSERT_TRUE(config.loadFromFile("test_empty_values.ini", err));
    
    // Empty values should be empty strings
    EXPECT_EQ(config.get("", "key1"), "");
    EXPECT_EQ(config.get("", "key2"), "");
}

// Test multiple equals signs in value
TEST_F(ConfigTest, MultipleEqualsInValue) {
    // Create file with multiple = in value
    std::ofstream ofs("test_multiple_equals.ini");
    ofs << "equation=x=y+z\n";
    ofs << "connection=host=localhost&port=3306\n";
    ofs.close();
    
    ASSERT_TRUE(config.loadFromFile("test_multiple_equals.ini", err));
    
    // Should keep everything after first =
    EXPECT_EQ(config.get("", "equation"), "x=y+z");
    // Note: semicolons are treated as comment markers, so using & instead
    EXPECT_EQ(config.get("", "connection"), "host=localhost&port=3306");
}
