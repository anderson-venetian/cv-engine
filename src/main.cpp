#include <print>
#include <vector>
#include <memory>
#include <cvengine/core/domain/cv.hpp>
#include <cvengine/core/domain/theme.hpp>
#include <cvengine/core/domain/rendered_document.hpp>
#include <cvengine/core/ports/i_data_repository.hpp>
#include <cvengine/core/ports/i_theme_repository.hpp>

namespace dom   = cvengine::core::domain;
namespace ports = cvengine::core::ports;
namespace common = cvengine::common;

// ----- Mock inline de IDataRepository: devuelve un CV hardcodeado -----
class InMemoryDataRepository final : public ports::IDataRepository {
public:
    [[nodiscard]] auto load(std::string_view /*identifier*/) const
        -> common::Result<dom::Cv> override {

        auto contact = dom::Contact::create(
            "Pedro Espinoza Trujillo", "Lima, Perú",
            "917 672 598", "pedro.espinozat@istpargentina.edu.pe"
        );
        if (!contact) return std::unexpected{contact.error()};

        auto profile = dom::ProfessionalProfile::create(
            "Técnico en Computación e Informática especializado en Service Desk "
            "y Help Desk N1/N2 en entornos BPO de alta demanda."
        );
        if (!profile) return std::unexpected{profile.error()};

        auto period = dom::Period::create("Mar 2026 — Abr 2026");
        if (!period) return std::unexpected{period.error()};

        auto ach = dom::Achievement::create("Operé el centro de cómputo en proceso electoral.");
        if (!ach) return std::unexpected{ach.error()};

        std::vector<dom::Achievement> achievements;
        achievements.push_back(*std::move(ach));

        auto exp = dom::Experience::create(
            "Operador de Centro de Cómputo", "ONPE", "Lima, Perú",
            *std::move(period), std::move(achievements)
        );
        if (!exp) return std::unexpected{exp.error()};

        auto edu = dom::Education::create(
            "Profesional Técnico en Computación e Informática",
            "IESTP Argentina", "Lima, Perú", "Dic 2020"
        );
        if (!edu) return std::unexpected{edu.error()};

        std::vector<dom::Experience> experiences;
        experiences.push_back(*std::move(exp));
        std::vector<dom::Education> education_list;
        education_list.push_back(*std::move(edu));

        return dom::Cv::create(
            *std::move(contact), *std::move(profile),
            {}, std::move(experiences), std::move(education_list), {}
        );
    }
};

// ----- Mock inline de IThemeRepository: devuelve un tema mínimo -----
class InMemoryThemeRepository final : public ports::IThemeRepository {
public:
    [[nodiscard]] auto load(std::string_view name) const
        -> common::Result<dom::Theme> override {
        dom::Theme::PropertyMap props{
            {"page.size",        "a4"},
            {"page.margin_top",  "2.1cm"},
            {"typography.base_size", "10.5pt"},
            {"colors.accent",    "#0066cc"}
        };
        return dom::Theme::create(name, std::move(props));
    }
};

auto main() -> int {
    std::println("cv-engine v2.0.0 — Fase 3: smoke test de ports");

    // Trabajamos a través de las interfaces, no de las clases concretas.
    std::unique_ptr<ports::IDataRepository> data_repo =
        std::make_unique<InMemoryDataRepository>();
    std::unique_ptr<ports::IThemeRepository> theme_repo =
        std::make_unique<InMemoryThemeRepository>();

    auto cv_r = data_repo->load("cv_pedro");
    if (!cv_r) {
        std::println("[FAIL] IDataRepository::load: {}", cv_r.error().message);
        return 1;
    }
    std::println("[OK] IDataRepository operativo. CV de: {}", cv_r->contact().name());

    auto theme_r = theme_repo->load("minimal");
    if (!theme_r) {
        std::println("[FAIL] IThemeRepository::load: {}", theme_r.error().message);
        return 1;
    }
    std::println("[OK] IThemeRepository operativo. Tema: {}", theme_r->name());
    std::println("[OK] Propiedades del tema: {}", theme_r->properties().size());
    std::println("[OK] theme.get(\"colors.accent\") = {}", theme_r->get("colors.accent"));
    std::println("[OK] theme.get(\"inexistente\", \"fallback\") = {}",
                 theme_r->get("inexistente", "fallback"));

    // Verificar RenderedDocument
    auto doc_r = dom::RenderedDocument::create(
        dom::RenderedDocument::Format::Latex,
        "\\documentclass{article}\\begin{document}Hello\\end{document}"
    );
    if (!doc_r) {
        std::println("[FAIL] RenderedDocument: {}", doc_r.error().message);
        return 1;
    }
    std::println("[OK] RenderedDocument: format={}, content_size={}",
                 dom::RenderedDocument::format_extension(doc_r->format()),
                 doc_r->content().size());

    std::println("\n✓ Fase 3 completa. Ports definidos y verificados con mocks.");
    return 0;
}
