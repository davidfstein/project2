#include <gtest/gtest.h>
#include "../src/datastructures/dataframe.h" 

#define GT_TRUE(a)   ASSERT_EQ((a),true)
#define GT_FALSE(a)  ASSERT_EQ((a),false)
#define GT_EQUALS(a, b)   ASSERT_EQ(a, b)
#define ASSERT_EXIT_ZERO(a)  \
  ASSERT_EXIT(a(), ::testing::ExitedWithCode(0), ".*")
#define ASSERT_EXIT_ONE(a)  \
  ASSERT_EXIT(a(), ::testing::ExitedWithCode(1), ".*")

void test_schema_correct_types() {
  char types[] = "IBFS";
  Schema s(types);
  for (int i = 0; i < strlen(types); i++) {
    GT_EQUALS(s.col_type(i), types[i]);
  }
  return;
}

void test_schema_empty_type_char() {
  char* types = "";
  Schema s(types);
  for (int i = 0; i < strlen(types); i++) {
    GT_EQUALS(s.col_type(i), types[i]);
  }
  return;
}

void test_add_column_empty_columns() {
  char* type = {"I"};
  Schema* s = new Schema();
  s->add_column(type, NULL);
  GT_EQUALS(s->col_type(0), 'I');
  delete s;
  return;
}

void test_add_column_full_columns() {
  char* types = {"II"};
  Schema s(types);
  s.add_column("S", nullptr);
  GT_EQUALS(s.col_type(0), 'I');
  GT_EQUALS(s.col_type(1), 'I');
  GT_EQUALS(s.col_type(2), 'S');
  return;
}

void test_add_column_name() {
  char* type = {"I"};
  Schema* s = new Schema();
  String* colString = new String("test");
  s->add_column(type, colString);
  String* matchString = new String("test");
  GT_TRUE(s->col_name(0)->equals(matchString));
  delete colString;
  delete matchString;
  delete s;
  return;
}

void test_add_column_empty_name() {
  char* type = {"I"};
  Schema* s = new Schema();
  String* colName = new String("");
  s->add_column(type, colName);
  String* matchName = new String("");
  GT_TRUE(s->col_name(0)->equals(matchName));
  delete colName;
  delete matchName;
  delete s;
  return;
}

void test_add_row_name() {
  char type = 'I';
  Schema* s = new Schema();
  String* rowName = new String("test");
  s->add_row(rowName);
  String* matchName = new String("test");
  GT_TRUE(s->row_name(0)->equals(matchName));
  delete rowName;
  delete matchName;
  delete s;
  return;
}

void test_add_row_empty_name() {
  char type = 'I';
  Schema* s = new Schema();
  String* rowName = new String("");
  s->add_row(rowName);
  String* matchName = new String("");
  GT_TRUE(s->row_name(0)->equals(matchName));
  delete rowName;
  delete matchName;
  delete s;
  return;
}

void test_set_row_name() {
  char type = 'I';
  Schema* s = new Schema();
  String* rowName = new String("test");
  String* newName = new String("new name");
  s->add_row(rowName);
  s->set_row_name(0, newName);
  String* matchName = new String("new name");
  GT_TRUE(s->row_name(0)->equals(matchName));
  delete rowName;
  delete newName;
  delete matchName;
  delete s;
  return;
}

void test_set_row_name_out_of_bounds() {
  char type = 'I';
  Schema* s = new Schema();
  String* rowName = new String("test");
  String* newName = new String("new name");
  s->add_row(rowName);
  s->set_row_name(1, newName);
  delete rowName;
  delete newName;
  delete s;
  return;
}

void test_set_row_name_nullptr() {
  char type = 'I';
  Schema* s = new Schema();
  String* rowName = new String("test");
  s->add_row(rowName);
  s->set_row_name(0, nullptr);
  delete rowName;
  delete s;
  return;
}

void test_row_width() {
  Schema* s = new Schema();
  for (int i = 0; i < 11 * 10; ++i) {
    String* name = new String("test");
    char type[] = "I";
    s->add_column(type, name);
    delete name;
  }
  GT_EQUALS(s->width(), (size_t) 11*10);
  delete s;
  return;
}

void test_row_length() {
  Schema* s = new Schema();
  for (int i = 0; i < 1000 * 1000; i++) {
    String* rowName = new String("H");
    s->add_row(rowName);
    delete rowName;
  }
  GT_EQUALS(s->length(), (size_t) 1000*1000);
  delete s;
  return;
}

// TEST(schema, t0) { test_schema_correct_types(); }
// TEST(schema, t2) { test_schema_empty_type_char(); }
// TEST(schema, t3) { test_add_column_empty_columns(); }
// TEST(schema, t4) { test_add_column_full_columns(); }
// TEST(schema, t5) { test_add_column_name(); }
// TEST(schema, t6) { test_add_column_empty_name(); }
// TEST(schema, t7) { test_add_row_name(); }
// TEST(schema, t8) { test_add_row_empty_name(); }
// TEST(schema, t9) { test_set_row_name(); }
// TEST(schema, t10) { ASSERT_EXIT_ONE(test_set_row_name_out_of_bounds); }
// TEST(schema, t11) { ASSERT_EXIT_ONE(test_set_row_name_nullptr); }
// TEST(schema, t12) { test_row_width(); }
// TEST(schema, t13) { test_row_length(); }

void test_make_int_column() {
  Schema* s = new Schema();
  DataFrame d(*s);
  Column* i = d.make_column('I');
  GT_EQUALS(i->type, 'I');
  delete i;
  return;
}

void test_make_string_column() {
  Schema* s = new Schema();
  DataFrame d(*s);
  Column* st = d.make_column('S');
  GT_EQUALS(st->type, 'S');
  delete st;
  return;
}

void test_make_bool_column() {
  Schema* s = new Schema();
  DataFrame d(*s);
  Column* b = d.make_column('B');
  GT_EQUALS(b->type, 'B');
  delete b;
  return;
}

void test_make_float_column() {
  Schema* s = new Schema();
  DataFrame d(*s);
  Column* f = d.make_column('F');
  GT_EQUALS(f->type, 'F');
  delete f;
  return;
}

void test_df_add_column() {
  Schema* s = new Schema();
  DataFrame d(*s);
  FloatColumn* f = (FloatColumn*) d.make_column('F');
  d.add_column(f, nullptr);
  GT_EQUALS(d.ncols(), 1);
  GT_EQUALS(d.schema->width(), 1);
  return;
}

void test_df_add_column_bad_length() {
  Schema* s = new Schema();
  DataFrame d(*s);
  Column* f = d.make_column('F');
  FloatColumn* fl = f->as_float();
  fl->push_back(float(10));
  d.add_column(fl, nullptr);
  delete f;
  return;
}

void test_type_at_position() {
  Schema* s = new Schema();
  DataFrame d(*s);
  Column* f = d.make_column('F');
  Column* b = d.make_column('B');
  d.add_column(f, nullptr);
  d.add_column(b, nullptr);
  d.checkTypeAtPosition(0, 'F');
  d.checkTypeAtPosition(1, 'B');
  return;
}

void test_wrong_type_at_position() {
  Schema* s = new Schema();
  DataFrame d(*s);
  Column* f = d.make_column('F');
  Column* b = d.make_column('B');
  d.add_column(f, nullptr);
  d.add_column(b, nullptr);
  d.checkTypeAtPosition(0, 'I');
  return;
}

void test_df_add_row() {
  Schema* s = new Schema("IBFFS");
  DataFrame d(*s);
  Row* r = new Row(*s);
  r->set(0, 9);
  r->set(1, true);
  r->set(2, (float) 10.0);
  r->set(3, (float) 10.0);
  String* rowVal = new String("Hi");
  r->set(4,rowVal); 
  d.add_row(*r);
  GT_EQUALS((size_t) 1, d.nrows());
  GT_EQUALS((size_t) 1, d.schema->length());
  delete r;
  delete rowVal;
  return;
}

void test_df_add_row_wrong_type() {
  Schema* s = new Schema("IBFFS");
  DataFrame d(*s);
  Row* r = new Row(*s);
  r->set(0, (float) 10.0);
  r->set(1, true);
  r->set(2, (float) 10.0);
  r->set(3, (float) 10.0);
  String* rowVal = new String("Hi");
  r->set(4,rowVal); 
  d.add_row(*r);
  delete r;
  delete rowVal;
  return;
}

void test_df_set_row() {
  Schema* s = new Schema("IBFFS");
  DataFrame d(*s);
  Row* r = new Row(*s);
  r->set(0, 9);
  r->set(1, (bool) true);
  r->set(2, (float) 10.0);
  r->set(3, (float) 10.0);
  String* rowVal = new String("Hi");
  r->set(4,rowVal); 
  d.add_row(*r);
  d.set(0, 0, (int) 1);
  d.set(1, 0, (bool) false);
  d.set(2, 0, (float) 1.0);
  d.set(3, 0, (float) 1.0);
  String* newVal = new String("Bye");
  d.set(4, 0, newVal);
  GT_EQUALS((int) 1, d.get_int(0, 0));
  GT_FALSE(d.get_bool(1, 0));
  GT_EQUALS((float) 1.0, d.get_float(2, 0));
  GT_EQUALS((float) 1.0, d.get_float(3, 0));
  String* matchVal = new String("Bye");
  GT_TRUE(d.get_string(4, 0)->equals(matchVal));
  delete r;
  delete rowVal;
  delete newVal;
  delete matchVal;
  return;
}

void test_df_set_row_lots() {
  Schema* s = new Schema("IBFFS");
  DataFrame d(*s);
  for (int i = 0; i < 50 * 1000 * 1000; i++) {
    Row* r = new Row(*s);
    r->set(0, 9);
    r->set(1, (bool) true);
    r->set(2, (float) 10.0);
    r->set(3, (float) 10.0);
    r->set(4, new String("Hi")); 
    d.add_row(*r);
    delete r;
  }
  d.set(0, 1000, (int) 1);
  d.set(1, 1000, (bool) false);
  d.set(2, 1000, (float) 1.0);
  d.set(3, 1000, (float) 1.0);
  d.set(4, 1000, new String("Bye"));
  GT_EQUALS((int) 1, d.get_int(0, 1000));
  GT_FALSE(d.get_bool(1, 1000));
  GT_EQUALS((float) 1.0, d.get_float(2, 1000));
  GT_EQUALS((float) 1.0, d.get_float(3, 1000));
  GT_TRUE(d.get_string(4, 1000)->equals(new String("Bye", 3)));
  return;
}

void test_nrows() {
  Schema* s = new Schema("IBFFS");
  DataFrame d(*s);
  Row* r = new Row(*s);
  r->set(0, 9);
  r->set(1, (bool) true);
  r->set(2, (float) 10.0);
  r->set(3, (float) 10.0);
  r->set(4, new String("Hi")); 
  for (int i = 0; i < 1000 * 1000; i++) {
    d.add_row(*r);
  }
  GT_EQUALS(d.nrows(), 1000 * 1000);
  exit(0);
}

void test_ncols() {
  Schema* s = new Schema("IBFFS");
  DataFrame d(*s);
  GT_EQUALS(d.ncols(), 5);
  exit(0);
}

// TEST(dataframe, t14) { test_make_int_column(); }
// TEST(dataframe, t15) { test_make_string_column(); }
// TEST(dataframe, t16) { test_make_bool_column(); }
// TEST(dataframe, t17) { test_make_float_column(); }
// TEST(dataframe, t18) { test_df_add_column(); }
// TEST(dataframe, t20) { test_type_at_position(); }
// TEST(dataframe, t21) { ASSERT_EXIT_ONE(test_wrong_type_at_position); }
// TEST(dataframe, t22) { test_df_add_row(); }
// TEST(dataframe, t23) { ASSERT_EXIT_ONE(test_df_add_row_wrong_type); }
// TEST(dataframe, t24) { test_df_set_row(); }
TEST(dataframe, t25) { test_df_set_row_lots(); }
// TEST(dataframe, t26) { ASSERT_EXIT_ZERO(test_nrows); }
// TEST(dataframe, t27) { ASSERT_EXIT_ZERO(test_ncols); }

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
};