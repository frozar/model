package com.example.demo.student;

import java.util.List;
import java.util.Optional;

import javax.transaction.Transactional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.web.bind.annotation.GetMapping;

// The @Transactional annotation avoid to use query (as in "findStudentByEmail"
// in "StudentRepository").
// It allows to use directly setter to write in DB. This annotation goes in
// management state.

@Service
public class StudentService {

  private final StudentRepository studentRepository;

  @Autowired
  public StudentService(StudentRepository studentRepository) {
    this.studentRepository = studentRepository;
  }

  @GetMapping
  public List<Student> getStudents() {
    return studentRepository.findAll();
  }

  public void addNewStudent(Student student) {
    Optional<Student> studentOptional = studentRepository
        .findStudentByEmail(student.getEmail());

    if (studentOptional.isPresent()) {
      throw new IllegalStateException("email taken");
    }
    studentRepository.save(student);
  }

  public void deleteStudent(Long studentId) {
    boolean exists = studentRepository.existsById(studentId);
    if (!exists) {
      throw new IllegalStateException(
          "student with id " + studentId + " does not exists");
    }
    studentRepository.deleteById(studentId);
  }

  @Transactional
  public void updateStudent(Long studentId, String name, String email) {
    Student student = studentRepository.findById(studentId).orElseThrow(() -> {
      throw new IllegalStateException(
          "student with id " + studentId + " does not exists");
    });

    if (name != null && name.length() > 0 && !student.getName().equals(name)) {
      student.setName(name);
    }
    if (email != null && email.length() > 0
        && !student.getEmail().equals(email)) {
      Optional<Student> studentOptional = studentRepository
          .findStudentByEmail(email);
      if(studentOptional.isPresent()) {
        throw new IllegalStateException("email taken");
      }
      student.setEmail(email);
    }
  }
}
