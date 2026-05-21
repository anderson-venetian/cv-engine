#include <print>
#include <vector>
#include <cvengine/core/domain/cv.hpp>

namespace dom = cvengine::core::domain;

auto main() -> int {
    std::println("cv-engine v2.0.0 — Fase 2: smoke test de dominio");

    // 1. Contact
    auto contact_r = dom::Contact::create(
        "Pedro Espinoza Trujillo",
        "Lima, Perú",
        "917 672 598",
        "pedro.espinozat@istpargentina.edu.pe"
    );
    if (!contact_r) {
        std::println("[FAIL] Contact: {}", contact_r.error().message);
        return 1;
    }

    // 2. Profile
    auto profile_r = dom::ProfessionalProfile::create(
        "Técnico en Computación e Informática especializado en Service Desk "
        "y Help Desk N1/N2 en entornos BPO de alta demanda. Certificado ITIL 4."
    );
    if (!profile_r) {
        std::println("[FAIL] Profile: {}", profile_r.error().message);
        return 1;
    }

    // 3. Skills
    std::vector<dom::SkillGroup> skills;
    if (auto s = dom::SkillGroup::create("Marcos y métricas", "ITIL 4, SLA, CSAT, AHT, FCR"); s) {
        skills.push_back(*std::move(s));
    }

    // 4. Una experiencia
    auto period_r = dom::Period::create("Mar 2026 — Abr 2026");
    if (!period_r) return 1;

    std::vector<dom::Achievement> achievements;
    if (auto a = dom::Achievement::create("Operé el centro de cómputo en proceso electoral."); a) {
        achievements.push_back(*std::move(a));
    }

    auto exp_r = dom::Experience::create(
        "Operador de Centro de Cómputo",
        "ONPE",
        "Lima, Perú",
        *std::move(period_r),
        std::move(achievements)
    );
    if (!exp_r) {
        std::println("[FAIL] Experience: {}", exp_r.error().message);
        return 1;
    }

    // 5. Educación
    auto edu_r = dom::Education::create(
        "Profesional Técnico en Computación e Informática",
        "IESTP Argentina",
        "Lima, Perú",
        "Dic 2020"
    );
    if (!edu_r) return 1;

    // 6. Agregado raíz
    std::vector<dom::Experience> experiences;
    experiences.push_back(*std::move(exp_r));
    std::vector<dom::Education> education_list;
    education_list.push_back(*std::move(edu_r));
    std::vector<dom::Certification> certs;

    auto cv_r = dom::Cv::create(
        *std::move(contact_r),
        *std::move(profile_r),
        std::move(skills),
        std::move(experiences),
        std::move(education_list),
        std::move(certs)
    );
    if (!cv_r) {
        std::println("[FAIL] Cv: {}", cv_r.error().message);
        return 1;
    }

    // 7. Verificación
    std::println("[OK] CV construido para: {}", cv_r->contact().name());
    std::println("[OK] Email: {}",              cv_r->contact().email());
    std::println("[OK] Profile len: {}",        cv_r->profile().text().size());
    std::println("[OK] Skill groups: {}",       cv_r->skills().size());
    std::println("[OK] Experiences: {}",        cv_r->experiences().size());
    std::println("[OK] Education entries: {}",  cv_r->education().size());
    std::println("[OK] Language: {}",           cv_r->language());

    // 8. Test negativo: debe fallar
    auto bad = dom::Contact::create("", "Lima", "", "no-email");
    if (bad) {
        std::println("[FAIL] Validación rota: Contact aceptó nombre vacío");
        return 1;
    }
    std::println("[OK] Validación rechaza Contact inválido: {}", bad.error().message);

    std::println("\n✓ Fase 2 completa. Dominio operativo.");
    return 0;
}
